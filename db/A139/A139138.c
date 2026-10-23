/*
 * A139138  Numbers divisible by at least two of their digits.
 * https://oeis.org/A139138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, dumb
 * Formula:   a(n) = the n‑th positive integer divisible by at least two of
 *            its decimal digits (duplicates of a digit count as separate
 *            divisors)
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *terms   = NULL;   /* cached sequence values */
    static size_t  cap    = 0;      /* capacity of terms[]   */
    static size_t  len    = 0;      /* number of stored terms */
    static seq_t  next    = 1;      /* last tested integer */

    if (n == 0)
        return 0;                    /* offset is 1; n=0 is undefined */

    /* Allocate initial storage if needed */
    if (!terms) {
        cap   = 32;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n */
    while (len < n) {
        next++;                       /* candidate number */

        seq_t x   = next;
        seq_t tmp = x;
        int count = 0;

        /* Count how many of its decimal digits divide x.  Digits may repeat
           and each occurrence is counted separately.  Zero digits are ignored. */
        while (tmp > 0) {
            int d = (int)(tmp % 10);   /* current last digit */
            tmp /= 10;

            if (d != 0 && x % d == 0) {
                count++;
                if (count >= 2)          /* early exit once satisfied */
                    break;
            }
        }

        if (count >= 2) {                /* store qualifying number */
            if (len >= cap) {            /* grow storage if needed */
                cap = cap ? cap * 2 : 32;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[len++] = x;
        }
    }

    return terms[n - 1];
}
