/*
 * A066407  Number of orthogonal arrays OA(6,n,2,1).
 * https://oeis.org/A066407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(10^n-15*4^n+45*2^n-40), n>=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    /* Handle offset; the sequence is defined for n >= 1. */
    if (n < 1)
        return 0;

    seq_t two_n  = seq_pow(2, n);
    seq_t four_n = seq_pow(4, n);
    seq_t ten_n  = seq_pow(10, n);

    seq_t inner = ten_n - 15 * four_n + 45 * two_n - 40;
    return two_n * inner;
}
