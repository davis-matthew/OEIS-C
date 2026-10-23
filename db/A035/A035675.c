/*
 * A035675  Number of partitions of n into parts 8k and 8k+4 with at least one
 *          part of each type.
 * https://oeis.org/A035675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n/4) - (n divisible by 8? p(n/8) : 0) - d(n/4) where
 *            p(k) is the unrestricted partition function and d(k) is the
 *            number of partitions of k into distinct parts (equivalently into
 *            odd parts)
 * Range:     u8 n <= 67, u16 n <= 175, u32 n <= 511, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static arrays holding partitions up to the current maximum */
static seq_t *p_arr = NULL;   /* standard partitions */
static seq_t *d_arr = NULL;   /* distinct-partitions (odd-partitions) */
static idx_t  arr_max = 0;    /* current maximum value stored in arrays */

/* Ensure p_arr[0..m] and d_arr[0..m] are computed. */
static void ensure_arrays(idx_t m)
{
    if (m <= arr_max) return;          /* already available */

    /* Create new arrays large enough to hold values 0..m */
    seq_t *new_p = (seq_t*)calloc((size_t)(m + 1), sizeof(seq_t));
    seq_t *new_d = (seq_t*)calloc((size_t)(m + 1), sizeof(seq_t));

    /* Base values */
    new_p[0] = 1;
    new_d[0] = 1;

    /* Fill standard partitions: coin summations for unlimited usage */
    for (idx_t k = 1; k <= m; ++k) {
        for (idx_t i = k; i <= m; ++i) {
            new_p[i] += new_p[i - k];
        }
    }

    /* Fill distinct-partitions: each coin used at most once (descending loop) */
    for (idx_t k = 1; k <= m; ++k) {
        for (idx_t i = m; i >= k; --i) {
            new_d[i] += new_d[i - k];
        }
    }

    /* Replace old arrays */
    free(p_arr);
    free(d_arr);
    p_arr = new_p;
    d_arr = new_d;
    arr_max = m;
}

seq_t a(idx_t n)
{
    /* Only multiples of 4 can be partitioned with parts of the form 8k or 8k+4. */
    if ((n & 3) != 0) return 0;

    idx_t m = n >> 2;           /* m = n/4 */
    ensure_arrays(m);

    /* Compute result: p(m) - p(m/2 if n divisible by 8) - d(m) */
    seq_t res = p_arr[m];

    if ((n & 7) == 0) {         /* n divisible by 8 */
        res -= p_arr[m >> 1];
    }

    res -= d_arr[m];
    return res;
}
