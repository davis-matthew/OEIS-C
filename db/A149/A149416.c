/*
 * A149416  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 1), (-1, 0,
 *          0), (-1, 1, 1), (1, 0, 1), (1, 1, -1)}.
 * https://oeis.org/A149416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   number of walks within N³ (first octant) starting at (0,0,0)
 *            with step set {(-1,-1,1),(-1,0,0),(-1,1,1),(1,0,1),(1,1,-1)}.
 *            The values satisfy a linear recurrence found by Berlekamp–Massey
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   mod_value_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mod_value_seq.h"

/* ---------- modular arithmetic helper ---------- */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1ULL % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1U;
    }
    return result;
}
static uint64_t mod_inv_uint64(uint64_t a, uint64_t mod) {
    return mod_pow_uint64(a, mod - 2, mod);
}

/* ---------- Berlekamp–Massey (mod prime) ---------- */
static void berlekamp_massey(const seq_t *s, size_t len, uint64_t mod,
                             uint64_t **rec_out, size_t *rec_len_out) {
    uint64_t *C = (uint64_t*)calloc(len, sizeof(uint64_t));
    uint64_t *B = (uint64_t*)calloc(len, sizeof(uint64_t));
    uint64_t *T;
    size_t L = 0, m = 1;
    uint64_t b = 1;
    C[0] = 1;
    B[0] = 1;

    for (size_t n = 0; n < len; n++) {
        uint64_t d = 0;
        for (size_t i = 0; i <= L; i++) {
            uint64_t si = mod_value_seq(s[n - i], mod);
            uint64_t term = (unsigned __int128)C[i] * si % mod;
            d += term;
            d %= mod;
        }
        if (d == 0) {
            m++;
            continue;
        }
        uint64_t coef = (unsigned __int128)d * mod_inv_uint64(b, mod) % mod;
        T = (uint64_t*)malloc((n + 1) * sizeof(uint64_t));
        memcpy(T, C, (n + 1) * sizeof(uint64_t));
        if (L + m > n) {
            for (size_t i = L + m; i <= n; i++) C[i] = 0;
        }
        for (size_t i = 0; i <= L; i++) {
            uint64_t val = (unsigned __int128)coef * B[i] % mod;
            C[i + m] += val;
            C[i + m] %= mod;
        }
        if (2 * L <= n) {
            L = n + 1 - L;
            memcpy(B, T, (n + 1) * sizeof(uint64_t));
            b = d;
            m = 1;
        } else {
            m++;
        }
        free(T);
    }

    *rec_len_out = L;
    *rec_out = (uint64_t*)malloc(L * sizeof(uint64_t));
    for (size_t i = 0; i < L; i++) {
        (*rec_out)[i] = (mod - C[i + 1]) % mod;   /* recurrence coefficient modulo mod */
    }
    free(C);
    free(B);
}

/* ---------- CRT to reconstruct signed integer coefficients ---------- */
static int64_t crt_integer(const uint64_t *residues, const uint64_t *mods,
                           size_t k, uint64_t prod) {
    unsigned __int128 x = 0;
    unsigned __int128 M = 1;
    for (size_t i = 0; i < k; i++) {
        uint64_t mod = mods[i];
        uint64_t r = residues[i];
        uint64_t cur_mod = (uint64_t)(x % mod);
        uint64_t invM = mod_inv_uint64(cur_mod, mod);
        uint64_t t = (r + mod - cur_mod) % mod;
        t = (unsigned __int128)t * invM % mod;
        x += (unsigned __int128)M * t;
        M *= mod;
    }
    /* x is in [0, prod) */
    int64_t coeff = (int64_t)x;
    if (coeff > (int64_t)(prod / 2)) coeff -= (int64_t)prod;
    return coeff;
}

/* ---------- Compute initial values up to N with brute force DP ---------- */
static void compute_initial_terms(seq_t *out, size_t N) {
    size_t stride = N + 1;
    size_t stride2 = stride * stride;
    size_t size = stride * stride * stride;
    seq_t *cur = (seq_t*)calloc(size, sizeof(seq_t));
    seq_t *nxt = (seq_t*)calloc(size, sizeof(seq_t));
    cur[0] = 1;
    out[0] = 1;

    /* step vectors */
    const int dx[5] = { -1, -1, -1,  1,  1 };
    const int dy[5] = { -1,  0,  1,  0,  1 };
    const int dz[5] = {  1,  0,  1,  1, -1 };

    for (size_t step = 0; step < N; step++) {
        memset(nxt, 0, size * sizeof(seq_t));
        seq_t sum = 0;
        for (int x = 0; x <= (int)step; x++) {
            for (int y = 0; y <= (int)step; y++) {
                for (int z = 0; z <= (int)step; z++) {
                    size_t idx = (size_t)x * stride2 + (size_t)y * stride + (size_t)z;
                    seq_t val = cur[idx];
                    if (!val) continue;
                    for (int k = 0; k < 5; k++) {
                        int nx = x + dx[k];
                        int ny = y + dy[k];
                        int nz = z + dz[k];
                        if (nx < 0 || ny < 0 || nz < 0) continue;
                        if (nx > (int)step + 1 || ny > (int)step + 1 || nz > (int)step + 1) continue;
                        size_t nidx = (size_t)nx * stride2 + (size_t)ny * stride + (size_t)nz;
                        nxt[nidx] += val;
                    }
                }
            }
        }
        /* accumulate total count for this step+1 */
        for (int x = 0; x <= (int)step + 1; x++) {
            for (int y = 0; y <= (int)step + 1; y++) {
                for (int z = 0; z <= (int)step + 1; z++) {
                    size_t idx = (size_t)x * stride2 + (size_t)y * stride + (size_t)z;
                    sum += nxt[idx];
                }
            }
        }
        out[step + 1] = sum;
        /* swap cur and nxt */
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }
    free(cur);
    free(nxt);
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n) {
    /* ---------- initialization ---------- */
    static int initialized = 0;
    enum { INIT_N = 60, PRIME_COUNT = 3 };
    static seq_t *seq_table = NULL;
    static long long *rec_table = NULL;
    static size_t rec_len = 0;

    if (!initialized) {
        /* compute initial terms */
        seq_table = (seq_t*)malloc((INIT_N + 1) * sizeof(seq_t));
        compute_initial_terms(seq_table, INIT_N);

        /* find recurrence over small primes */
        uint64_t primes[PRIME_COUNT] = { 1000003ULL, 1000033ULL, 1000037ULL };
        uint64_t prod = 1ULL;
        for (int i = 0; i < PRIME_COUNT; i++) prod *= primes[i];

        uint64_t **residues = (uint64_t**)malloc(PRIME_COUNT * sizeof(uint64_t*));
        size_t *rec_lens = (size_t*)malloc(PRIME_COUNT * sizeof(size_t));

        for (int i = 0; i < PRIME_COUNT; i++) {
            berlekamp_massey(seq_table, INIT_N + 1, primes[i],
                             &residues[i], &rec_lens[i]);
        }

        /* ensure all lists have same length (pad with zeros) */
        size_t L = rec_lens[0];
        for (int i = 1; i < PRIME_COUNT; i++) {
            if (rec_lens[i] > L) L = rec_lens[i];
        }
        for (int i = 0; i < PRIME_COUNT; i++) {
            if (rec_lens[i] < L) {
                residues[i] = (uint64_t*)realloc(residues[i], L * sizeof(uint64_t));
                for (size_t j = rec_lens[i]; j < L; j++) residues[i][j] = 0;
                rec_lens[i] = L;
            }
        }

        rec_table = (long long*)malloc(L * sizeof(long long));
        for (size_t j = 0; j < L; j++) {
            uint64_t res_arr[PRIME_COUNT];
            for (int i = 0; i < PRIME_COUNT; i++) res_arr[i] = residues[i][j];
            int64_t c = crt_integer(res_arr, primes, PRIME_COUNT, prod);
            rec_table[j] = c;
        }
        rec_len = L;

        /* free temporary data */
        for (int i = 0; i < PRIME_COUNT; i++) free(residues[i]);
        free(residues);
        free(rec_lens);

        initialized = 1;
    }

    /* ---------- answer for n ---------- */
    if ((size_t)n <= INIT_N) return seq_table[(size_t)n];

    size_t k = rec_len;
    seq_t *buffer = (seq_t*)malloc(k * sizeof(seq_t));
    for (size_t i = 0; i < k; i++) buffer[i] = seq_table[i];

    for (size_t i = k; i <= (size_t)n; i++) {
        __int128 acc = 0;
        for (size_t j = 0; j < k; j++) {
            acc += (__int128)rec_table[j] * (__int128)buffer[(i - 1 - j) % k];
        }
        buffer[i % k] = (seq_t)acc;
    }
    seq_t result = buffer[(size_t)n % k];
    free(buffer);
    return result;
}
