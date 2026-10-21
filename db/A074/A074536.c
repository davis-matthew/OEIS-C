/*
 * A074536  a(n) = 2^n + 4^n + 9^n.
 * https://oeis.org/A074536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 4^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Exponentiation by squaring – works for any seq_t type. */
static seq_t ipow(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow2  = ipow((seq_t)2, n);
    seq_t pow4  = ipow((seq_t)4, n);
    seq_t pow9  = ipow((seq_t)9, n);
    return pow2 + pow4 + pow9;
}
