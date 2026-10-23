/*
 * A167998  Numbers k with property that first digit of 5*k = last digit of k.
 * https://oeis.org/A167998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A167998: Numbers k such that the first decimal digit of 5*k
   equals the last decimal digit of k.  a(1) is the smallest such k.
   The function scans k from 1 upwards until it finds the n‑th match. */

#include <stdbool.h>   /* for bool if needed, harmless in C */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1 – behaviour irrelevant */

    idx_t count = 0;                 /* how many matches found so far */
    idx_t k     = 1;                 /* candidate k, 1‑based */

    while (true) {
        /* last decimal digit of k */
        idx_t last = k % 10U;

        /* 5*k */
        seq_t m = (seq_t)k * ((seq_t)5U);

        /* first decimal digit of 5*k */
        seq_t first = m;
        while (first >= 10U) {
            first /= 10U;
        }

        /* check the property */
        if ((seq_t)last == first) {
            ++count;
            if (count == n) {
                return (seq_t)k;    /* found n‑th term */
            }
        }

        ++k;
    }
}
