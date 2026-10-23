/*
 * A155608  a(n) = 10^n + 3^n - 1.
 * https://oeis.org/A155608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n + 3^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) {
            result = result * base;
        }
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow10 = pow_seq((seq_t)10, n);
    seq_t pow3  = pow_seq((seq_t)3,  n);
    return pow10 + pow3 - (seq_t)1;
}
