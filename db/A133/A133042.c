/*
 * A133042  Cubes of partition numbers.
 * https://oeis.org/A133042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n)^3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 24, u64 n <= 66, u128 n <= 206,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the partition number p(n) for n >= 0 and returns p(n) ^ 3. */
seq_t a(idx_t n)
{
    /* Static cache of partition numbers. */
    static seq_t *part = NULL;       /* array of p(i) */
    static idx_t  current_max = 0;   /* largest n for which part[i] is filled */

    /* Ensure the cache array is allocated and contains at least part[0] */
    if (part == NULL) {
        part = (seq_t *)malloc(sizeof(seq_t));
        if (part == NULL) return 0;          /* allocation failure -> zero */
        part[0] = (seq_t)1;                  /* p(0) = 1 */
        current_max = 0;
    }

    /* Extend the cache if necessary */
    if (n > current_max) {
        seq_t *tmp = (seq_t *)realloc(part, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failure -> zero */
        part = tmp;
        /* Compute partition numbers for indices current_max+1 .. n */
        for (idx_t i = current_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; ; ++k) {
                /* Generalized pentagonal numbers */
                idx_t g1 = k * (3 * k - 1) / 2;
                if (g1 > i) break;
                /* sign: +1 for odd k, -1 for even k */
                if ((k & 1) == 1) { /* odd k: add */
                    sum += part[i - g1];
                } else {           /* even k: subtract */
                    sum -= part[i - g1];
                }
                idx_t g2 = k * (3 * k + 1) / 2;
                if (g2 > i) continue;
                if ((k & 1) == 1) { /* odd k: add */
                    sum += part[i - g2];
                } else {           /* even k: subtract */
                    sum -= part[i - g2];
                }
            }
            part[i] = sum;
        }
        current_max = n;
    }

    seq_t p = part[n];
    return p * p * p;
}
