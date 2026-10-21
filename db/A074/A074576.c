/*
 * A074576  a(n) = 5^n + 8^n + 9^n.
 * https://oeis.org/A074576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n + 8^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes base^exp using binary exponentiation. */
static seq_t pow_uint(idx_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    idx_t e = exp;

    while (e) {
        if (e & 1) result *= b;
        e >>= 1;
        if (!e) break;
        b *= b;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p5 = pow_uint(5, n);
    seq_t p8 = pow_uint(8, n);
    seq_t p9 = pow_uint(9, n);
    return p5 + p8 + p9;
}
