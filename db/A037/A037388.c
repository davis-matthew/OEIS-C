/*
 * A037388  Numbers k such that every base-4 digit of k is a base-6 digit of
 *          k.
 * https://oeis.org/A037388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k such that every base-4 digit of k is a base-6 digit of
 *            k
 * Range:     u8 n <= 19, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool */

/* Check whether the integer k satisfies the definition:
   every digit in base-4 representation of k is also a digit
   in base-6 representation of k. */
static bool good(seq_t k)
{
    seq_t temp;
    unsigned int mask4 = 0U, mask6 = 0U;

    /* Process base-4 digits */
    temp = k;
    while (temp > (seq_t)0) {
        unsigned int d = (unsigned int)(temp % (seq_t)4);
        mask4 |= 1U << d;
        temp /= (seq_t)4;
    }

    /* Process base-6 digits */
    temp = k;
    while (temp > (seq_t)0) {
        unsigned int d = (unsigned int)(temp % (seq_t)6);
        mask6 |= 1U << d;
        temp /= (seq_t)6;
    }

    /* All base-4 digits must appear in base-6 digits */
    return (mask4 & ~mask6) == 0U;
}

/* External function required by the test harness */
seq_t a(idx_t n)
{
    /* n is 1‑based; no n==0 case is needed, but handle it safely */
    if (n == (idx_t)0) return (seq_t)0;

    seq_t k = (seq_t)1;
    idx_t count = (idx_t)0;

    while (1) {
        if (good(k)) {
            count++;
            if (count == n) return k;
        }
        k++;   /* auto‑increment to next candidate */
    }
}
