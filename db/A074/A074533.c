/*
 * A074533  a(n) = 2^n + 4^n + 6^n.
 * https://oeis.org/A074533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 4^n + 6^n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes base^exp using binary exponentiation.  Works for all
   integer-like types `seq_t` and `idx_t` provided by the harness. */
static inline seq_t
pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;          /* identity element */
    while (exp != 0) {
        if (exp & 1UL)                /* if the lowest set bit is 1 */
            result *= base;
        exp >>= 1UL;                  /* shift right to process the next bit */
        if (exp)                      /* avoid an unnecessary multiply at the end */
            base *= base;
    }
    return result;
}

/* Sequence generator as required by the harness. */
seq_t
a(idx_t n)
{
    return pow_seq_t((seq_t)2, n) +
           pow_seq_t((seq_t)4, n) +
           pow_seq_t((seq_t)6, n);
}
