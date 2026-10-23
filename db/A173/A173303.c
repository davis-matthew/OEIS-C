/*
 * A173303  Row sums of triangle A173302.
 * https://oeis.org/A173303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=0, 2^k <= n+1} A000041(n + 1 - 2^k)
 * Range:     u8 n <= 13, u16 n <= 37, u32 n <= 116, u64 n <= 394,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000041 is the partition function. */

/* standard includes */
#include <stdint.h>
#include <stdlib.h>

/* partition cache */
static seq_t *p_cache = NULL;          /* array p_cache[i] = p(i) */
static idx_t  p_len   = 0;              /* current length of cache */

/* make sure p_cache has size at least (max_n+1) */
static void ensure_partitions(idx_t max_n)
{
    if (max_n < p_len) return;            /* already enough */

    idx_t new_len = (p_len == 0) ? 64 : p_len;
    while (new_len <= max_n) new_len <<= 1;

    seq_t *new_cache = (seq_t *)realloc(p_cache, new_len * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    p_cache = new_cache;

    /* initialise new entries with 0 */
    for (idx_t i = p_len; i < new_len; ++i) p_cache[i] = 0;

    /* base case */
    if (p_len == 0) {
        p_cache[0] = 1;
        p_len = 1;
    }

    /* compute partition numbers up to max_n */
    for (idx_t n = p_len; n <= max_n; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1;; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > n) break;
            if (k & 1) sum += p_cache[n - g1];
            else      sum -= p_cache[n - g1];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > n) continue;
            if (k & 1) sum += p_cache[n - g2];
            else      sum -= p_cache[n - g2];
        }
        p_cache[n] = sum;
    }
    p_len = max_n + 1;
}

/* --------------------------------------------------------------- */
/* public function required by the specification */

seq_t a(idx_t n)
{
    /* need partition numbers up to n */
    ensure_partitions(n);

    seq_t sum = 0;
    for (idx_t k = 0;; ++k) {
        /* 2^k */
        idx_t pw = (idx_t)1 << k;
        if (pw > n + 1) break;
        idx_t m = n + 1 - pw;   /* index for partition */
        sum += p_cache[m];
    }
    return sum;
}
