/*
 * A035363  Number of partitions of n into even parts.
 * https://oeis.org/A035363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 87, u32 n <= 255, u64 n <= 833,
 *            u128 n <= 2001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035363: Number of partitions of n into even parts
 * Formula: a(n) = 0 if n is odd; a(n) = p(n/2) otherwise,
 *            where p(m) is the unrestricted partition function.
 * This implementation computes p(m) incrementally using the
 * generalized pentagonal number recurrence, which is O(m√m)
 * and works for any build that provides unsigned integer type
 * seq_t. The function is memoised so equal or larger arguments are
 * computed once and reused.  It accepts calls in any order.
 */

#include <stdlib.h>

/* External function required by the build */
seq_t a(idx_t n)
{
    /* Return 0 for odd n */
    if (n & 1ULL) return 0;

    /* Compute value for even n */
    idx_t m = n >> 1;   /* n/2, always integer since n is even */

    /* Static cache of partition values p[0..current_max] */
    static seq_t *part = NULL;   /* array of partition numbers */
    static idx_t part_size = 0;  /* size allocated for part array  */
    static idx_t part_current = 0; /* highest index already computed */

    /* Grow the array if necessary */
    if (m + 1 > part_size) {
        seq_t *tmp = (seq_t *)realloc(part, (m + 1) * sizeof(seq_t));
        part = tmp;
        part_size = m + 1;
    }

    /* Initialise part[0] on first use */
    if (part_current == 0 && part != NULL) {
        part[0] = (seq_t)1;
    }

    /* Extend partition table up to m if required */
    if (m > part_current) {
        for (idx_t i = part_current + 1; i <= m; ++i) {
            seq_t pos = 0;   /* sum of terms with positive sign */
            seq_t neg = 0;   /* sum of terms with negative sign */
            for (idx_t k = 1; ; ++k) {
                idx_t pent1 = k * (3 * k - 1) / 2;
                idx_t pent2 = k * (3 * k + 1) / 2;

                if (pent1 > i && pent2 > i) break;

                if (pent1 <= i) {
                    if (k & 1) pos += part[i - pent1];
                    else       neg += part[i - pent1];
                }
                if (pent2 <= i) {
                    if (k & 1) pos += part[i - pent2];
                    else       neg += part[i - pent2];
                }
            }
            /* pos - neg performs unsigned subtraction, which
               gives the correct result modulo 2^w for the
               chosen seq_t width.  Overflows are inevitable
               once p(i) exceeds the type's capacity, but the
               caller expects exactly that behaviour. */
            part[i] = pos - neg;
        }
        part_current = m;
    }

    return part[m];
}
