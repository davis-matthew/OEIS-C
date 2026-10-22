/*
 * A000189  Number of solutions to x^3 == 0 (mod n).
 * https://oeis.org/A000189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   multiplicative with a(p^e) = p^floor(2e/3)
 * Range:     u8 n <= 4095, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            unsigned e = 0, k;
            while (m % p == 0) { m /= p; e++; }
            for (k = 0; k < 2 * e / 3; k++)
                r *= (seq_t)p;
        }
    }
    /* a leftover prime has exponent 1, so it contributes p^0 = 1 */
    return r;
}
