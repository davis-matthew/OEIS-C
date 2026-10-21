/*
 * A087292  Number of pairs of polynomials (f,g) in GF(3)[x] satisfying 1 <=
 *          deg(f) < =n, 1 <= deg(g) <= n and gcd(f,g) = 1.
 * https://oeis.org/A087292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6 * (3^n - 1)^2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 3^n using binary exponentiation */
    seq_t base = 3;          /* base = 3 */
    seq_t pow3 = 1;          /* result accumulator */
    idx_t exp = n;           /* exponent in idx_t */

    while (exp > 0) {
        if (exp & 1) pow3 *= base; /* multiply by current base if bit is set */
        base *= base;              /* square the base */
        exp >>= 1;                 /* shift right to process next bit */
    }

    /* Apply the formula a(n) = 6 * (3^n - 1)^2 */
    seq_t t = pow3 - 1;
    seq_t t2 = t * t;
    return 6 * t2;
}
