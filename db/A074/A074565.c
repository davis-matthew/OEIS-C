/*
 * A074565  a(n) = 4^n + 6^n + 7^n.
 * https://oeis.org/A074565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 6^n + 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Exponentiation by squaring – safe for all seq_t sizes that can
   represent the final result.  Intermediate products are kept in the
   same type as the result; this is acceptable because the result
   itself fits in seq_t.  */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1)
            result = result * base;
        exp >>= 1;
        if (exp)
            base = base * base;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t val4 = pow_seq((seq_t)4, n);
    seq_t val6 = pow_seq((seq_t)6, n);
    seq_t val7 = pow_seq((seq_t)7, n);
    return val4 + val6 + val7;
}
