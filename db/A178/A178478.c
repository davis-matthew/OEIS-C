/*
 * A178478  Permutations of 12345678: Numbers having each of the decimal
 *          digits 1..8 exactly once, and no other digit.
 * https://oeis.org/A178478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, easy, base
 * Formula:   a(n) = nth lexicographic permutation of the digits 1..8, written
 *            as an 8‑digit decimal number
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return 0 for invalid index (the sequence is defined for n ≥ 1). */
    if (n == 0) return (seq_t)0;

    /* 0‑based index.  Wrap around if n is larger than 8!; this keeps
       the function well‑defined for any n.  (The OEIS b‑file contains
       only the first 8! terms.) */
    idx_t idx = (n - 1) % 40320;          /* 8! = 40320 */

    /* Precomputed factorials up to 8! (unsigned 64‑bit). */
    static const idx_t fact[9] = {
        1, 1, 2, 6, 24, 120, 720, 5040, 40320
    };

    /* Remaining digits to choose from, always sorted ascending. */
    int rem[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    seq_t result = 0;

    /* Build the permutation digit by digit. */
    for (int pos = 0; pos < 8; ++pos) {
        int remaining = 8 - pos;
        idx_t f = fact[remaining - 1];
        idx_t d = idx / f;          /* index of the next digit */
        idx   %= f;                 /* remainder for next positions */

        int digit = rem[d];
        result = result * 10 + digit;

        /* Remove the chosen digit from the list. */
        for (int j = d; j < remaining - 1; ++j) {
            rem[j] = rem[j + 1];
        }
    }

    return result;
}
