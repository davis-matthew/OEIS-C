/*
 * A086444  Sequence associated with palindromic structures.
 * https://oeis.org/A086444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n/3 + 3^n/2 + 5^n/6 (n ≥ 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow2 = power_seq_t((seq_t)2, n + 1);
    seq_t pow3 = power_seq_t((seq_t)3, n + 1);
    seq_t pow5 = power_seq_t((seq_t)5, n);
    seq_t sum  = pow2 + pow3 + pow5;
    return sum / (seq_t)6;
}
