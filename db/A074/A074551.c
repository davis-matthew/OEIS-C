/*
 * A074551  a(n) = 3^n + 4^n + 9^n.
 * https://oeis.org/A074551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 4^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes base^exp for integer types seq_t and uint64_t exponent
 * using binary exponentiation. Works for both fixed‑width and
 * arbitrary‑precision seq_t implementations that support ``+*,`` */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp) {
        if (exp & (idx_t)1) {
            result *= b;
        }
        b *= b;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    return pow_seq_t((seq_t)3, n) +        /* 3^n */
           pow_seq_t((seq_t)4, n) +        /* 4^n */
           pow_seq_t((seq_t)9, n);        /* 9^n */
}
