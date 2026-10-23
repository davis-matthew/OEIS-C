/*
 * A191820  A178478(n)/9.
 * https://oeis.org/A191820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, easy, base
 * Formula:   a(n) is the (n‑1)-th lexicographically ordered permutation of
 *            the digits 1–8, divided by 9 (which is always an integer because
 *            the sum of the digits is 36, a multiple of 9)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* factorials 0! … 7! – these values are small enough to fit in idx_t. */
static const uint64_t _fact[8] = {
    1ULL, 1ULL, 2ULL, 6ULL, 24ULL, 120ULL, 720ULL, 5040ULL
};

seq_t a(idx_t n)
{
    /* The sequence is defined for 1 ≤ n ≤ 8! = 40320. */
    if (n == 0 || n > (_fact[7] * 8))
        return 0;

    idx_t rank = n - 1;                /* 0‑based index in permutation list */
    uint8_t digits[8] = {1,2,3,4,5,6,7,8};
    seq_t number = 0;

    for (int pos = 0; pos < 8; ++pos) {
        int remaining = 7 - pos;                     /* digits left after this position */
        idx_t fact = _fact[remaining];               /* (remaining)!   */
        idx_t idx = rank / fact;                     /* which digit goes here */
        uint8_t d = digits[idx];

        /* Append the chosen digit to the number.  seq_t may be
           arbitrary‑precision; the operation is defined for all builds. */
        number = number * 10 + d;

        /* Remove the used digit from the array. */
        for (int j = idx; j < remaining; ++j)
            digits[j] = digits[j + 1];

        /* Update the rank for the remaining positions. */
        rank %= fact;
    }

    /* All generated numbers are divisible by 9, so integer division is exact. */
    return number / 9;
}
