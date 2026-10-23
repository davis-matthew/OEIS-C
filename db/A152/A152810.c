/*
 * A152810  Let the binary expansion of n be n = Sum_{k} 2^{r_k}, let e(n) be
 *          the number of r_k's that are even, o(n) the number that are odd;
 *          sequence gives odd n such that e(n) > o(n) and e(n)-o(n) == 1 or 2
 *          (mod 6).
 * https://oeis.org/A152810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth odd integer n such that e(n) > o(n) and
 *            e(n)-o(n) == 1 or 2 (mod 6), where e(n) counts bits of n at even
 *            positions and o(n) counts bits at odd positions (LSB is position
 *            0)
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Return the nth term of sequence A152810. */
seq_t a(idx_t n)
{
    /* On first call initialise static storage. */
    static seq_t *terms = NULL;      /* array of terms found */
    static size_t len      = 0;      /* number of terms stored */
    static size_t cap      = 0;      /* capacity of the array */
    static idx_t  cand     = 1;      /* next odd integer to test */

    if (len == 0) {                  /* first execution */
        cap  = 256;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms) exit(1);        /* failed allocation */
    }

    /* Generate terms until we have at least n terms. */
    while (len < n) {
        /* Ensure cand is odd (not strictly necessary for
           subsequent steps because we always add 2, but kept
           for clarity). */
        if ((cand & 1) == 0) cand++;

        /* Compute difference d = e(n)-o(n). */
        long long d = 0;            /* difference can be negative */
        idx_t pos = 0;              /* bit position (LSB = 0) */
        idx_t temp = cand;          /* candidate as idx_t (fits 64-bit) */
        while (temp > 0) {
            if ((temp & 1) != 0) {
                if ((pos & 1) == 0) d++;   /* even position */
                else                 d--;   /* odd position */
            }
            temp >>= 1;
            pos++;
        }

        /* Check the required properties. */
        if (d > 0 && (d % 6 == 1 || d % 6 == 2)) {
            /* Store the term. */
            if (len >= cap) {
                cap *= 2;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (!terms) exit(1); /* failed allocation */
            }
            terms[len++] = (seq_t)cand;
        }

        /* Advance to the next odd integer. */
        cand += 2;
    }

    return terms[n - 1];
}
