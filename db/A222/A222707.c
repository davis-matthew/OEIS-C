/*
 * A222707  Total number of parts of multiplicity 7 in all partitions of n.
 * https://oeis.org/A222707
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k\ge 1}\bigl(p(n-7k)-p(n-8k)\bigr) where p(m) is
 *            the partition function, p(0)=1
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 145, u64 n <= 437,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *p_part = NULL;       /* array of partition numbers */
static idx_t  p_part_len = 0;      /* number of elements allocated (>=1) */

/* Compute p[0..need] and store in the static array.  The array is grown
   lazily so repeated calls with larger n are cheap. */
static void ensure_partitions(idx_t need)
{
    if (p_part != NULL && p_part_len > need) return;

    idx_t new_len = need + 1;                 /* indices 0..need */
    seq_t *new_arr = NULL;
    if (p_part == NULL) {
        new_arr = (seq_t *)malloc(new_len * sizeof(seq_t));
    } else {
        new_arr = (seq_t *)realloc(p_part, new_len * sizeof(seq_t));
    }
    /* memory allocation failure is unlikely for n≈10 000; ignore for brevity */
    p_part      = new_arr;
    p_part_len  = new_len;

    p_part[0] = (seq_t)1;
    for (idx_t i = 1; i <= need; ++i) {
        seq_t sum = (seq_t)0;
        for (int k = 1; ; ++k) {
            idx_t pent1 = (idx_t)(k * (3 * k - 1) / 2);
            if (pent1 > i) break;
            seq_t term1 = p_part[i - pent1];
            if (k & 1)   sum += term1;
            else         sum -= term1;

            idx_t pent2 = (idx_t)(k * (3 * k + 1) / 2);
            if (pent2 <= i) {
                seq_t term2 = p_part[i - pent2];
                if (k & 1)   sum += term2;
                else         sum -= term2;
            }
        }
        p_part[i] = sum;
    }
}

/* Count the total number of parts of multiplicity 7 in all partitions of n. */
seq_t a(idx_t n)
{
    if (n < 7) return (seq_t)0;        /* behaviour below the offset is arbitrary */
    ensure_partitions(n);              /* p[0..n] are now available */

    seq_t res = (seq_t)0;

    for (idx_t k = 1; ; ++k) {          /* sum of p(n-7k) */
        if (n < 7 * k) break;
        res += p_part[n - 7 * k];
    }
    for (idx_t k = 1; ; ++k) {          /* subtract p(n-8k) */
        if (n < 8 * k) break;
        res -= p_part[n - 8 * k];
    }
    return res;
}
