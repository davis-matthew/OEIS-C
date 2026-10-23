/*
 * A189023  Apostol's fourth-order Mobius (Moebius) function mu_4(n).
 * https://oeis.org/A189023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   Apostol's fourth-order Möbius function μ₄(n): μ₄(n) = 1 if all
 *            prime exponents e ≤ 3 -1 if an odd number of prime exponents are
 *            exactly 4 0 if any prime exponent e ≥ 5
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* for safety, not needed in tests */

    seq_t result = 1;           /* start with multiplicative identity */
    uint64_t x = n;

    /* trial division to find prime factors and their exponents */
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            int exp = 0;
            while (x % p == 0) {
                ++exp;
                x /= p;
            }
            if (exp >= 5) return 0;         /* exponent too large */
            if (exp == 4) result = -result; /* toggle sign for exp==4 */
        }
    }

    /* if remaining factor > 1 it is prime with exponent 1 (≤3) */
    /* nothing to do for exponent 1 */

    return result;
}
