/*
 * A053108  Expansion of 1/(1 - 9*x)^9.
 * https://oeis.org/A053108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n * binomial(n+8, 8)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes 9^n * C(n+8,8) using only integer arithmetic.
   The binomial coefficient is evaluated multiplicatively to keep
   intermediate values small, and the power is computed by
   exponentiation by squaring.  All operations are performed on
   seq_t, which may be an unsigned integer or an arbitrary‑precision
   type.  */
seq_t a(idx_t n)
{
    /* C(n+8,8) = ∏_{i=1}^{8} (n+9-i) / i   */
    seq_t bin = 1;
    for (idx_t i = 1; i <= 8; ++i) {
        seq_t num = (seq_t)(n + 9 - i);     /* numerator term */
        bin = bin * num / (seq_t)i;        /* exact division */
    }

    /* 9^n via exponentiation by squaring */
    seq_t pow9 = 1;
    seq_t base = 9;
    idx_t e = n;
    while (e > 0) {
        if (e & 1) pow9 = pow9 * base;
        base = base * base;
        e >>= 1;
    }

    return bin * pow9;
}
