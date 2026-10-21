/*
 * A066216  Noncube numbers which are sums of cubes of some subset of
 *          divisors.
 * https://oeis.org/A066216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 324, u32 n <= 1600, u64 n <= 1600,
 *            u128 n <= 1600, big any n
 * Helpers:   cmp_seq_desc, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A066216 – Non‑cube numbers which are sums of cubes of a subset
 * of their divisors.  The function a(idx_t n) returns the n‑th term
 * (n ≥ 1).  A lazy, memoised sieve is used: numbers are tested in
 * increasing order until enough terms are found. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>                    /* for qsort */

/* utility supplied by the build */
#include "../utils/floor_cube_root.h"  /* seq_t floor_cube_root(seq_t) */
#include "../utils/cmp_seq_desc.h"

/* --- prime sieve (up to 2 000 000) ------------------------------------ */
#define PRIME_LIST_MAX 148933           /* number of primes ≤ 2 000 000 */
static uint32_t prime_list[PRIME_LIST_MAX];
static size_t  prime_count = 0;
static bool    primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    size_t limit = 2000000U;
    bool *is_comp = (bool *)calloc(limit + 1UL, sizeof(bool));
    if (!is_comp) return;               /* shouldn't happen */

    for (uint32_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            prime_list[prime_count++] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                    is_comp[(size_t)j] = true;
            }
        }
    }
    free(is_comp);
    primes_ready = true;
}

/* --- generate cubes of divisors -------------------------------------- */
#define MAX_CUBES 10000                 /* safe upper bound for cubes */
static seq_t cubes_arr[MAX_CUBES];
static seq_t suffix_arr[MAX_CUBES];

static void gen_cubes_rec(const uint64_t *primes, const uint8_t *exps,
                          size_t pf_cnt, size_t idx, uint64_t cur,
                          uint64_t limit, size_t *cnt)
{
    if (idx == pf_cnt) {
        if (cur <= limit && *cnt < MAX_CUBES) {
            seq_t c = (seq_t)cur * cur * cur;   /* cur³ fits in seq_t */
            cubes_arr[(*cnt)++] = c;
        }
        return;
    }
    uint64_t p = primes[idx];
    uint8_t e = exps[idx];
    uint64_t val = cur;
    for (int i = 0; i <= (int)e; ++i) {
        if (val > limit) break;
        gen_cubes_rec(primes, exps, pf_cnt, idx + 1, val, limit, cnt);
        if (i == e) break;
        val *= p;
    }
}

/* --- subset‑sum search ----------------------------------------------- */
typedef struct {
    seq_t *cubes;   /* sorted descending */
    size_t cnt;
    seq_t *suffix;  /* suffix sums */
    seq_t target;
} dfs_context_t;

static bool dfs_subset(int pos, seq_t sum, const dfs_context_t *ctx)
{
    if (sum == ctx->target) return true;
    if (pos == (int)ctx->cnt) return false;
    seq_t remaining = ctx->target - sum;
    if (ctx->suffix[pos] < remaining) return false;   /* cannot reach target */

    seq_t c = ctx->cubes[pos];
    if (c <= remaining) {
        if (dfs_subset(pos + 1, sum + c, ctx)) return true;
    }
    return dfs_subset(pos + 1, sum, ctx);
}

/* --- check whether a number satisfies the property -------------------- */
static bool satisfies(uint64_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t r = floor_cube_root(n_seq);
    seq_t r3 = r * r * r;
    if (r3 == n_seq) return false;            /* skip perfect cubes */

    /* factor n by trial division using the small prime list */
    uint64_t tmp = n;
    uint64_t primes[64];
    uint8_t exps[64];
    size_t pc = 0;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        if (tmp % p == 0) {
            uint8_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            primes[pc] = p;
            exps[pc] = cnt;
            ++pc;
        }
    }
    if (tmp > 1) {
        primes[pc] = tmp;
        exps[pc] = 1;
        ++pc;
    }

    /* generate cubes of divisors ≤ cube root */
    uint64_t max_div = (uint64_t)r;            /* r is small (≤ 2^22) */
    size_t cubes_cnt = 0;
    gen_cubes_rec(primes, exps, pc, 0, 1, max_div, &cubes_cnt);

    if (cubes_cnt == 0) return false;          /* shouldn't happen */

    /* sort cubes descending for pruning */
    qsort(cubes_arr, cubes_cnt, sizeof(seq_t), cmp_seq_desc);

    /* compute suffix sums */
    suffix_arr[cubes_cnt - 1] = cubes_arr[cubes_cnt - 1];
    for (int i = (int)cubes_cnt - 2; i >= 0; --i)
        suffix_arr[i] = suffix_arr[i + 1] + cubes_arr[i];

    dfs_context_t ctx = {cubes_arr, cubes_cnt, suffix_arr, n_seq};
    return dfs_subset(0, (seq_t)0, &ctx);
}

/* --- cache of terms --------------------------------------------------- */
static seq_t *term_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t candidate = 1;

static void ensure_cache_space(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        term_cache = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* --- public sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, safe return */

    init_primes();

    /* initialise cache on first call */
    if (!term_cache) {
        cache_cap = 64;
        term_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t x = candidate++;
        if (x == 0) continue;             /* overflow guard */

        if (satisfies(x)) {
            ensure_cache_space();
            term_cache[cache_len++] = (seq_t)x;
        }
    }

    return term_cache[n - 1];
}
