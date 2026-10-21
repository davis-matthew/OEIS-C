/*
 * A216194  a(n) = Smallest b for which the base b representation of n
 *          contains at least one 2 (or 0 if no such base exists).
 * https://oeis.org/A216194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = smallest base b >= 2 such that the representation of n in
 *            base b contains a digit 2, * or 0 if no such base exists
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes A216194 for 1 <= n */
seq_t a(idx_t n)
{
    /* Handle small n explicitly */
    if (n <= 4) {
        if (n == 2) return (seq_t)3;
        return (seq_t)0;
    }

    /* For n >= 5 we know that base (n-2) yields the representation "12",
       hence a(n) > 0.  Search for the minimal base. */
    for (idx_t b = 2; b <= n - 2; ++b) {
        idx_t tmp = n;
        bool has2 = false;
        while (tmp > 0) {
            if (tmp % b == 2) { has2 = true; break; }
            tmp /= b;
        }
        if (has2) return (seq_t)b;
    }
    /* Should never reach here for n >= 5, but return 0 for safety */
    return (seq_t)0;
}
