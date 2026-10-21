/*
 * A067120  a(n) = floor(X/Y) where X = concatenation of first n odd numbers
 *          in increasing order and Y = n-th triangular number.
 * https://oeis.org/A067120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, less
 * Formula:   a(n) = floor(concat(odd numbers 1..(2n-1)) / (n(n+1)/2))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digits.h"   /* digits(idx_t) */
#include "../utils/pow10.h"   /* pow10(idx_t) */

extern seq_t a(idx_t n)
{
    /* Triangular number Y = n(n+1)/2 */
    seq_t Yseq = (seq_t)n * (seq_t)(n + 1) / 2;
    /* Yseq fits in 64 bits for n <= 10000 (max ~5e8) */
    uint64_t Yval = (uint64_t)Yseq;

    /* Precomputed powers of ten up to 10^5 (odd numbers up to 19999 have at most 5 digits) */
    static const idx_t POW10_SMALL[6] = {1, 10, 100, 1000, 10000, 100000};

    seq_t q = 0;          /* quotient so far */
    uint64_t rem = 0;     /* remainder while processing digits */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t odd = 2 * k - 1;          /* current odd number */
        int d = digits(odd);           /* number of decimal digits of odd */

        /* Process digits of 'odd' from most significant to least significant */
        for (int pos = d - 1; pos >= 0; --pos) {
            idx_t pow = POW10_SMALL[pos];
            idx_t digit = (odd / pow) % 10;

            /* Update remainder with the new digit */
            rem = rem * 10 + (uint64_t)digit;

            /* If we have accumulated at least one non‑zero quotient digit or
               the remainder is large enough to contribute to the quotient,
               compute the next quotient digit. */
            if (q || rem >= Yval) {
                seq_t remseq = (seq_t)rem;
                seq_t qdigit = remseq / Yseq;          /* floor(rem / Y) */
                q = q * 10 + qdigit;
                rem = (uint64_t)(remseq % Yseq);
            }
        }
    }

    return q;   /* floor(X / Y) */
}
