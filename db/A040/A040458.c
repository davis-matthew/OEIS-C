/*
 * A040458  Continued fraction for sqrt(480).
 * https://oeis.org/A040458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   a(0) = 21; for n ≥ 1 let e = v₂(n) (the exponent of the prime 2
 *            in n). Then a(n) = 1 if e = 0, a(n) = 9 if e = 1, and a(n) = 42
 *            if e ≥ 2
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence is multiplicative: a(m·n) = a(m)·a(n) when gcd(m,n)=1. */

seq_t a(idx_t n)
{
    if (n == 0) return 21;              /* special offset value */
    uint64_t m = (uint64_t)n;           /* use native 64-bits for bit tests */
    if ((m & 1ULL) == 0) {              /* n is even */
        int e = 0;
        while ((m & 1ULL) == 0) {       /* count trailing zeros = exponent of 2 */
            e++;
            m >>= 1;
        }
        if (e == 1) return 9;          /* 2¹ */
        else          return 42;       /* 2ᵉ, e ≥ 2 */
    }
    /* n is odd: exponent of 2 is 0 */
    return 1;
}
