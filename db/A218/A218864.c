/*
 * A218864  Numbers of the form 9*k^2 + 8*k, k an integer.
 * https://oeis.org/A218864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18*n*(n - 1) - 7*(-1)^n*(2*n - 1) - 7)/8
 * Range:     u8 n <= 10, u16 n <= 170, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use a wider unsigned type for intermediate calculations to avoid overflow.
       n ≤ 10 000 in our tests, so 64‑bit arithmetic is more than enough. */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long term1 = 18ULL * nn * (nn - 1ULL);
    unsigned long long term2 = 7ULL * (2ULL * nn - 1ULL);
    unsigned long long numerator;

    if ((n & 1ULL) == 0) {          /* n is even */
        numerator = term1 - term2 - 7ULL;
    } else {                        /* n is odd */
        numerator = term1 + term2 - 7ULL;
    }

    unsigned long long result = numerator / 8ULL;
    return (seq_t)result;
}
