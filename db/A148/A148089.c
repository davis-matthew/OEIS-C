/*
 * A148089  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 1), (-1, 1,
 *          1), (0, 0, 1), (1, 0, -1)}.
 * https://oeis.org/A148089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   decode_key, mod_swap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Traverse 3‑D walks in the first octant with steps
   {(-1,-1,1), (-1,1,1), (0,0,1), (1,0,-1)}.
   The function computes a(n) number of such walks of length n.
   It first pre‑computes terms up to N0 = 200 by breadth‑first search
   using a small open‑addressing hash table, then finds a linear
   recurrence over a large prime and uses it to extend the sequence
   for any n.  The implementation is fully generic for any seq_t /
   idx_t supplied by the build environment.  No external libraries
   are required. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/decode_key.h"
#include "../utils/mod_swap.h"

/* ---------- modular arithmetic for recurrence detection ---------- */
#define PRIME 2305843009213693951ULL   /* 2^61-1, a Mersenne prime */

static inline uint64_t mod_add(uint64_t a, uint64_t b)
{
    __uint128_t t = (__uint128_t)a + b;
    return (uint64_t)(t % PRIME);
}

static inline uint64_t mod_sub(uint64_t a, uint64_t b)
{
    return mod_add(a, PRIME - b % PRIME);
}

static inline uint64_t mod_mul(uint64_t a, uint64_t b)
{
    __uint128_t t = (__uint128_t)a * b;
    return (uint64_t)(t % PRIME);
}

static inline uint64_t mod_pow(uint64_t a, uint64_t e)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1ULL) r = mod_mul(r, a);
        a = mod_mul(a, a);
        e >>= 1;
    }
    return r;
}

static inline uint64_t mod_inv(uint64_t a)
{
    return mod_pow(a, PRIME - 2ULL);
}

/* ---------- Gaussian elimination over the prime field ---------- */
#define MAX_ORDER 30 /* enough for all discovered recursions */

static int solve_modular_system(int k, uint64_t (*mat)[MAX_ORDER + 1], uint64_t *rhs, uint64_t *sol)
{
    int row = 0;
    for (int col = 0; col < k; ++col)
    {
        int sel = -1;
        for (int i = row; i < k; ++i)
        {
            if (mat[i][col] != 0)
            {
                sel = i;
                break;
            }
        }
        if (sel == -1) return 0;

        if (sel != row)
        {
            for (int j = col; j <= k; ++j)
                mod_swap(&mat[sel][j], &mat[row][j]);
            mod_swap(&rhs[sel], &rhs[row]);
        }

        uint64_t inv = mod_inv(mat[row][col]);
        for (int j = col; j <= k; ++j)
            mat[row][j] = mod_mul(mat[row][j], inv);
        rhs[row] = mod_mul(rhs[row], inv);

        for (int i = 0; i < k; ++i)
        {
            if (i == row) continue;
            uint64_t factor = mat[i][col];
            if (factor == 0) continue;
            for (int j = col; j <= k; ++j)
                mat[i][j] = mod_sub(mat[i][j], mod_mul(factor, mat[row][j]));
            rhs[i] = mod_sub(rhs[i], mod_mul(factor, rhs[row]));
        }
        ++row;
    }

    for (int i = 0; i < k; ++i)
        sol[i] = rhs[i];
    return 1;
}

/* ---------- Hash table for state counts (x,y,z) ---------- */
#define HASH_BITS 20
#define HASH_SIZE (1 << HASH_BITS)

typedef struct {
    uint32_t key;   /* (z<<16)|(x<<8)|y  ,  16 bits per coord */
    seq_t    val;   /* exact count */
    uint64_t mod;   /* count modulo PRIME */
    unsigned used; /* flag 0/1 */
} Entry;

static inline size_t hash_key(uint32_t k)
{
    return (size_t)((k * 2654435761u) >> (32 - HASH_BITS));
}

static inline uint32_t encode_key(uint32_t z, uint32_t x, uint32_t y)
{
    return (z << 16) | (x << 8) | y;
}

/* ---------- Pre‑computation and recurrence discovery ---------- */
#define N0 200 /* number of terms computed by exhaustive DP */

static seq_t *global_cache = NULL;
static size_t cache_len = 0, cache_cap = 0; /* cache for a(0)…a(n) */

static int  rec_order = 0;          /* order of found recurrence  */
static seq_t *rec_coeff = NULL;     /* coefficients c1…ck */

/* ensure global_cache has capacity for n+1 terms */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(global_cache, new_cap * sizeof(seq_t));
    if (!tmp) return; /* memory failure – caller will fail */
    global_cache = tmp;
    cache_cap = new_cap;
}

/* extend cache using recurrence to reach n */
static void expand_cache(idx_t n)
{
    if (rec_order == 0) return; /* no recurrence yet */
    if (n < (idx_t)cache_len) return;
    ensure_cache_capacity(n);
    for (idx_t i = (idx_t)cache_len; i <= n; ++i)
    {
        seq_t val = 0;
        for (int j = 0; j < rec_order; ++j)
            val += rec_coeff[j] * global_cache[i - j - 1];
        global_cache[i] = val;
    }
    cache_len = (size_t)n + 1;
}

static void precompute_and_find_recurrence(void)
{
    /* allocate hash tables */
    Entry *curr = (Entry *)calloc(HASH_SIZE, sizeof(Entry));
    Entry *next = (Entry *)calloc(HASH_SIZE, sizeof(Entry));

    int *curr_keys = (int *)calloc(HASH_SIZE, sizeof(int));
    int *next_keys = (int *)calloc(HASH_SIZE, sizeof(int));
    int curr_used_cnt = 0, next_used_cnt = 0;

    /* initial state (0,0,0) */
    uint32_t key0 = encode_key(0, 0, 0);
    size_t h0 = hash_key(key0);
    curr[h0].used = 1;
    curr[h0].key = key0;
    curr[h0].val = (seq_t)1;
    curr[h0].mod = 1;
    curr_keys[curr_used_cnt++] = h0;

    seq_t *a_seq = (seq_t *)malloc((N0 + 1) * sizeof(seq_t));
    uint64_t *a_mod = (uint64_t *)malloc((N0 + 1) * sizeof(uint64_t));
    a_seq[0] = (seq_t)1;
    a_mod[0] = 1;

    for (int step = 0; step < N0; ++step)
    {
        /* reset next map */
        for (int i = 0; i < next_used_cnt; ++i)
            next[ next_keys[i] ].used = 0;
        next_used_cnt = 0;

        /* iterate current states */
        for (int idx_pos = 0; idx_pos < curr_used_cnt; ++idx_pos)
        {
            int idx = curr_keys[idx_pos];
            seq_t cur_val = curr[idx].val;
            uint64_t cur_mod = curr[idx].mod;

            uint32_t z, x, y;
            decode_key(curr[idx].key, &z, &x, &y);

            /* step A: (-1,-1,+1) */
            if (x >= 1 && y >= 1)
            {
                uint32_t nk = encode_key(z + 1, x - 1, y - 1);
                size_t h = hash_key(nk);
                while (next[h].used && next[h].key != nk)
                    h = (h + 1) & (HASH_SIZE - 1);
                if (!next[h].used)
                {
                    next[h].used = 1;
                    next[h].key = nk;
                    next[h].val = 0;
                    next[h].mod = 0;
                    next_keys[next_used_cnt++] = h;
                }
                next[h].val += cur_val;
                next[h].mod = mod_add(next[h].mod, cur_mod);
            }

            /* step B: (-1,+1,+1) */
            if (x >= 1)
            {
                uint32_t nk = encode_key(z + 1, x - 1, y + 1);
                size_t h = hash_key(nk);
                while (next[h].used && next[h].key != nk)
                    h = (h + 1) & (HASH_SIZE - 1);
                if (!next[h].used)
                {
                    next[h].used = 1;
                    next[h].key = nk;
                    next[h].val = 0;
                    next[h].mod = 0;
                    next_keys[next_used_cnt++] = h;
                }
                next[h].val += cur_val;
                next[h].mod = mod_add(next[h].mod, cur_mod);
            }

            /* step C: (0,0,+1) */
            {
                uint32_t nk = encode_key(z + 1, x, y);
                size_t h = hash_key(nk);
                while (next[h].used && next[h].key != nk)
                    h = (h + 1) & (HASH_SIZE - 1);
                if (!next[h].used)
                {
                    next[h].used = 1;
                    next[h].key = nk;
                    next[h].val = 0;
                    next[h].mod = 0;
                    next_keys[next_used_cnt++] = h;
                }
                next[h].val += cur_val;
                next[h].mod = mod_add(next[h].mod, cur_mod);
            }

            /* step D: (+1,0,-1) */
            if (z >= 1)
            {
                uint32_t nk = encode_key(z - 1, x + 1, y);
                size_t h = hash_key(nk);
                while (next[h].used && next[h].key != nk)
                    h = (h + 1) & (HASH_SIZE - 1);
                if (!next[h].used)
                {
                    next[h].used = 1;
                    next[h].key = nk;
                    next[h].val = 0;
                    next[h].mod = 0;
                    next_keys[next_used_cnt++] = h;
                }
                next[h].val += cur_val;
                next[h].mod = mod_add(next[h].mod, cur_mod);
            }
        }

        /* compute a(step+1) as sum of all next states */
        seq_t sum_seq = 0;
        uint64_t sum_mod = 0;
        for (int i = 0; i < next_used_cnt; ++i)
        {
            int idx = next_keys[i];
            sum_seq += next[idx].val;
            sum_mod = mod_add(sum_mod, next[idx].mod);
        }
        a_seq[step + 1] = sum_seq;
        a_mod[step + 1] = sum_mod;

        /* swap current and next maps */
        Entry *tmp = curr; curr = next; next = tmp;
        int *tmp_keys = curr_keys; curr_keys = next_keys; next_keys = tmp_keys;
        int tmp_cnt = curr_used_cnt; curr_used_cnt = next_used_cnt; next_used_cnt = tmp_cnt;
    }

    /* ---------- search for linear recurrence ---------- */
    int found = 0;
    for (int k = 1; k <= MAX_ORDER && !found; ++k)
    {
        uint64_t mat[MAX_ORDER][MAX_ORDER + 1];
        uint64_t rhs[MAX_ORDER];
        uint64_t sol[MAX_ORDER];

        for (int i = 0; i < k; ++i)
        {
            for (int j = 0; j < k; ++j)
                mat[i][j] = a_mod[k + i - (j + 1)];
            rhs[i] = a_mod[k + i];
        }

        if (!solve_modular_system(k, mat, rhs, sol)) continue;

        /* convert modular solution to signed integer and check */
        seq_t coeffs[MAX_ORDER];
        int ok = 1;
        for (int j = 0; j < k; ++j)
        {
            uint64_t v = sol[j];
            int64_t signed_v = (v >= PRIME/2) ? (int64_t)(v - PRIME) : (int64_t)v;
            if (signed_v < 0) { ok = 0; break; }
            coeffs[j] = (seq_t)signed_v;
        }
        if (!ok) continue;

        /* verify on all pre‑computed terms */
        for (int n = k; n <= N0; ++n)
        {
            seq_t pred = 0;
            for (int j = 0; j < k; ++j)
                pred += coeffs[j] * a_seq[n - j - 1];
            if (pred != a_seq[n]) { ok = 0; break; }
        }
        if (!ok) continue;

        /* recurrence found */
        rec_order = k;
        rec_coeff = (seq_t *)malloc(k * sizeof(seq_t));
        for (int j = 0; j < k; ++j) rec_coeff[j] = coeffs[j];
        found = 1;
    }

    /* ---------- initialise global cache ---------- */
    cache_cap = N0 + 1;
    global_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    for (int i = 0; i <= N0; ++i) global_cache[i] = a_seq[i];
    cache_len = N0 + 1;

    /* ---------- clean up ---------- */
    free(curr);
    free(next);
    free(curr_keys);
    free(next_keys);
    free(a_seq);
    free(a_mod);
}

/* ---------- public sequence function --------------------------------- */
seq_t a(idx_t n)
{
    /* lazy initialization */
    static int initialized = 0;
    if (!initialized)
    {
        precompute_and_find_recurrence();
        initialized = 1;
    }

    if (n < (idx_t)cache_len) return global_cache[(size_t)n];
    expand_cache(n);
    return global_cache[(size_t)n];
}
