/*
 * A074518  a(n) = 1^n + 5^n + 8^n.
 * https://oeis.org/A074518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1^n + 5^n + 8^n (OEIS A074518)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t power_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t value  = base;
    while (exp) {
        if (exp & 1U) {
            result = result * value;
        }
        value = value * value;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 1^n is always 1, 5^n and 8^n are computed by exponentiation by squaring. */
    seq_t pow5 = power_seq_t((seq_t)5, n);
    seq_t pow8 = power_seq_t((seq_t)8, n);
    return (seq_t)1 + pow5 + pow8;
}
