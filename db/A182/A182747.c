/*
 * A182747  Bisection (odd part) of number of partitions that do not contain 1
 *          as a part A002865.
 * https://oeis.org/A182747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(2*n+1) – p(2*n), where p(k) is the partition function
 *            (A000041)
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 75, u64 n <= 232,
 *            u128 n <= 782, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided types seq_t (value type) and idx_t (index type). */
#include <stddef.h>
#include <stdlib.h>

/* Cache for partition numbers p(0)…p(max). */
static seq_t *partitions = NULL;
static size_t  part_cap   = 0;   /* capacity of the array */
static size_t  part_len   = 0;   /* number of computed entries */

/* Ensure that partitions[0] … partitions[upto] are computed. */
static void
ensure_partitions(idx_t upto)
{
    /* nothing to do if already computed */
    if (upto < (idx_t)part_len)
        return;

    /* Resize the array if necessary. */
    size_t needed = (size_t)(upto + 1);   /* inclusive */
    if (needed > part_cap) {
        size_t new_cap = part_cap ? part_cap : 2;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *new_arr = (seq_t *)realloc(partitions, new_cap * sizeof(seq_t));
        if (!new_arr) abort();           /* out of memory */
        partitions = new_arr;
        part_cap   = new_cap;
    }

    /* Initialize p(0) = 1 if this is the first call. */
    if (part_len == 0) {
        partitions[0] = (seq_t)1;
        part_len = 1;
    }

    /* Compute partition numbers for k = part_len … upto. */
    for (idx_t k = (idx_t)part_len; k <= upto; ++k) {
        seq_t sum_pos = (seq_t)0;
        seq_t sum_neg = (seq_t)0;

        for (idx_t m = 1; ; ++m) {
            /* Generalized pentagonal numbers */
            idx_t g1 = m * (3 * m - 1) / 2;
            idx_t g2 = m * (3 * m + 1) / 2;

            /* If both exceed k, we are done. */
            if (g1 > k && g2 > k)
                break;

            /* Process g1 if it fits. */
            if (g1 <= k) {
                seq_t val = partitions[k - g1];
                if ((m & 1) == 1)
                    sum_pos += val;   /* odd m → positive sign */
                else
                    sum_neg += val;   /* even m → negative sign */
            }

            /* Process g2 if it fits. */
            if (g2 <= k) {
                seq_t val = partitions[k - g2];
                if ((m & 1) == 1)
                    sum_pos += val;
                else
                    sum_neg += val;
            }
        }

        partitions[k] = sum_pos - sum_neg;   /* unsigned subtraction, guaranteed non‑negative */
    }

    part_len = needed;
}

/* Public function required by the problem statement. */
seq_t
a(idx_t n)
{
    /* We need p(2*n+1) and p(2*n). */
    idx_t upto = (n << 1) + 1;   /* 2*n + 1, safe for idx_t */
    ensure_partitions(upto);
    return partitions[upto] - partitions[upto - 1];
}
