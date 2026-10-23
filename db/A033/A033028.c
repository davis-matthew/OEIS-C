/*
 * A033028  Numbers whose base-15 expansion has no run of digits with length <
 *          2.
 * https://oeis.org/A033028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose base‑15 expansion has no run
 *            of identical digits of length < 2
 * Range:     u8 n <= 15, u16 n <= 267, u32 n <= 1100, u64 n <= 1100,
 *            u128 n <= 1100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* small helper: binomial coefficient C(n,r) (fits in size_t) */
static inline size_t binom_int(int n, int r)
{
    if (r > n - r) r = n - r;
    size_t res = 1;
    for (int i = 1; i <= r; ++i) {
        res = res * (n - r + i) / i;
    }
    return res;
}

/* powers of 14, up to a reasonable exponent (enough for n <= 10000) */
static const size_t POW14[20] = {
    1ULL, 14ULL, 196ULL, 2744ULL, 38416ULL, 537824ULL,
    7529536ULL, 105413504ULL, 1475789056ULL, 20661046784ULL,
    289254654976ULL, 4052521557504ULL, 56735302314056ULL,
    794133332610784ULL, 11105173008692576ULL,
    155263042122795584ULL, 2175267589721910016ULL,
    30493711766062380224ULL, 427608364625866242736ULL,
    5981513030004229644176ULL
};

/* number of valid base‑15 strings of length k */
static size_t count_terms_len(int k)
{
    size_t total = 0;
    for (int m = 1; 2 * m <= k; ++m) {
        size_t comb = binom_int(k - m - 1, m - 1);
        size_t pow  = POW14[m];
        total += comb * pow;
    }
    return total;
}

/* recursive generator: builds all strings of length k with run length >= 2
   and writes them directly into the output buffer. */
static void dfs_gen(int pos, int k, int prev_digit, int run_len,
                    seq_t cur, seq_t *out, size_t *cnt)
{
    if (pos == k) {
        if (run_len >= 2) {
            out[(*cnt)++] = cur;
        }
        return;
    }

    int start = (pos == 0) ? 1 : 0;
    for (int d = start; d <= 14; ++d) {
        seq_t next_val = cur * (seq_t)15 + (seq_t)d;
        if (pos == 0) {
            dfs_gen(pos + 1, k, d, 1, next_val, out, cnt);
        } else if (d == prev_digit) {
            dfs_gen(pos + 1, k, prev_digit, run_len + 1, next_val, out, cnt);
        } else {
            if (run_len >= 2) {
                dfs_gen(pos + 1, k, d, 1, next_val, out, cnt);
            }
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of terms stored */
    static idx_t cache_cap = 0;  /* allocated capacity */
    static int next_len = 2;     /* next base‑15 digit length to generate */

    assert(n >= 1);  /* behaviour for n<1 is undefined */

    /* Ensure cache contains at least n terms */
    while (cache_len < n) {
        int k = next_len;

        size_t cnt_k = count_terms_len(k);

        /* Expand cache if necessary */
        if (cache_cap < cache_len + cnt_k) {
            seq_t *tmp = (seq_t*)realloc(cache,
                                         (size_t)(cache_len + cnt_k) * sizeof(seq_t));
            if (!tmp) {
                abort();  /* allocation failure */
            }
            cache = tmp;
            cache_cap = cache_len + cnt_k;
        }

        /* Generate terms of length k directly into cache */
        size_t written = 0;
        dfs_gen(0, k, -1, 0, (seq_t)0, cache + cache_len, &written);
        assert(written == cnt_k);
        cache_len += written;
        ++next_len;
    }

    return cache[n - 1];
}
