/*
 * A227486  The Wiener index of the nanostar dendrimer defined pictorially as
 *          G[n] in the M. H. Khalifeh et al. reference.
 * https://oeis.org/A227486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 502 + 2^(n)*(2800*n + 8716) + 4^(n)*(8000*n - 3920)
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 23, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2(idx_t e)
{
    seq_t result = 1;
    seq_t base   = 2;
    idx_t exp = e;
    while (exp)
    {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t two_n   = pow2(n);               /* 2^n */
    seq_t four_n  = two_n * two_n;         /* 4^n = (2^n)^2 */

    seq_t term1 = two_n * ((seq_t)2800 * (seq_t)n + (seq_t)8716);
    seq_t term2 = four_n * ((seq_t)8000 * (seq_t)n - (seq_t)3920);

    return (seq_t)502 + term1 + term2;
}
