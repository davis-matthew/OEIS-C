/*
 * A053154  Number of 2-element intersecting families (with not necessarily
 *          distinct sets) of an n-element set.
 * https://oeis.org/A053154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4^n - 3^n + 2^n - 1)/2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A053154. */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t power = base;
    while (exp > 0)
    {
        if (exp & 1) result *= power;
        power *= power;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow4 = pow_seq_t((seq_t)4, n);
    seq_t pow3 = pow_seq_t((seq_t)3, n);
    seq_t pow2 = pow_seq_t((seq_t)2, n);

    seq_t temp = pow4 - pow3 + pow2 - (seq_t)1;
    return temp / (seq_t)2;
}
