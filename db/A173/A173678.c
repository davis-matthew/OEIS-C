/*
 * A173678  Number of ways of writing n as a sum of 4 nonnegative cubes.
 * https://oeis.org/A173678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ordered 4‑tuples (x,y,z,w) of nonnegative
 *            integers with x³ + y³ + z³ + w³ = n (OEIS A173678)
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute integer cube root: largest r such that r³ ≤ n. */
static idx_t int_cube_root(idx_t n)
{
    idx_t r = 0;
    while (1) {
        idx_t nxt = r + 1;
        seq_t cube = (seq_t)nxt * nxt * nxt;
        if (cube > (seq_t)n) break;
        r = nxt;
    }
    return r;
}

/* Pre‑computation up to LIMIT. */
#define LIMIT 10000ull          /* maximum n for which we pre‑store results */

static seq_t *pre_cache = NULL; /* array of size LIMIT+1*/
static idx_t  pre_cache_len = 0;

/* Fill pre_cache with the number of representations for all 0 ≤ n ≤ LIMIT. */
static void build_pre_cache(void)
{
    if (pre_cache != NULL) return;

    pre_cache_len = LIMIT + 1;
    pre_cache = (seq_t *)malloc(pre_cache_len * sizeof(seq_t));
    if (!pre_cache) return; /* out of memory – unlikely */

    for (idx_t i = 0; i < pre_cache_len; ++i) pre_cache[i] = 0;

    idx_t m = int_cube_root(LIMIT);
    /* Pre‑compute cubes to avoid recomputation */
    seq_t *cubes = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= m; ++i) cubes[i] = (seq_t)i * i * i;

    for (idx_t a = 0; a <= m; ++a) {
        seq_t a3 = cubes[a];
        for (idx_t b = 0; b <= m; ++b) {
            seq_t ab3 = a3 + cubes[b];
            if (ab3 > LIMIT) continue;
            for (idx_t c = 0; c <= m; ++c) {
                seq_t abc3 = ab3 + cubes[c];
                if (abc3 > LIMIT) continue;
                for (idx_t d = 0; d <= m; ++d) {
                    seq_t s = abc3 + cubes[d];
                    if (s <= LIMIT) {
                        pre_cache[(idx_t)s] += 1;
                    }
                }
            }
        }
    }
    free(cubes);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initialise cache if not yet done. */
    if (!pre_cache) build_pre_cache();

    /* Fast path for n within pre‑computed range. */
    if (n < pre_cache_len) return pre_cache[n];

    /* For larger n compute on the fly.
       Number of iterations is small because n grows slowly.   */
    idx_t m = int_cube_root(n);
    seq_t total = 0;
    for (idx_t a = 0; a <= m; ++a) {
        seq_t a3 = (seq_t)a * a * a;
        if (a3 > n) break;
        for (idx_t b = 0; b <= m; ++b) {
            seq_t ab3 = a3 + (seq_t)b * b * b;
            if (ab3 > n) break;
            for (idx_t c = 0; c <= m; ++c) {
                seq_t abc3 = ab3 + (seq_t)c * c * c;
                if (abc3 > n) break;
                for (idx_t d = 0; d <= m; ++d) {
                    seq_t s = abc3 + (seq_t)d * d * d;
                    if (s > n) break;
                    if (s == n) total += 1;
                }
            }
        }
    }
    return total;
}
