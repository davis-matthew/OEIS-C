/*
 * A067556  Terms in the decimal expansion of 1/(7*2^n) before the block of
 *          decimals 142857 (the period of 1/7) appears.
 * https://oeis.org/A067556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (5^n * 10^(n mod 6) - 1) / 7
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 5^n */
    seq_t pow5 = pow_seq_t((seq_t)5, n);

    /* 10^(n mod 6) */
    idx_t mod6 = n % (idx_t)6;
    seq_t pow10 = pow_seq_t((seq_t)10, mod6);

    /* (5^n * 10^(n mod 6) - 1) / 7 */
    seq_t numerator = pow5 * pow10 - (seq_t)1;
    seq_t res = numerator / (seq_t)7;
    return res;
}
