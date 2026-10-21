/*
 * A057011  Number of conjugacy classes of subgroups of index 5 in free group
 *          of rank n.
 * https://oeis.org/A057011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            120^(n-1)-24^(n-1)-12^(n-1)+6^(n-1)+5^(n-1)+4^(n-1)-2^(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for safety return 0 for n = 0. */
    if (n == 0) return (seq_t)0;

    idx_t e = n - 1;

    seq_t p120 = pow_seq_t((seq_t)120, e);
    seq_t p24  = pow_seq_t((seq_t)24,  e);
    seq_t p12  = pow_seq_t((seq_t)12,  e);
    seq_t p6   = pow_seq_t((seq_t)6,   e);
    seq_t p5   = pow_seq_t((seq_t)5,   e);
    seq_t p4   = pow_seq_t((seq_t)4,   e);
    seq_t p2   = pow_seq_t((seq_t)2,   e);

    return p120 - p24 - p12 + p6 + p5 + p4 - p2;
}
