/*
 * A016269  Number of monotone Boolean functions of n variables with 2
 *          mincuts. Also number of Sperner systems with 2 blocks.
 * https://oeis.org/A016269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) * (1 + 2^(n+2)) – 3^(n+2) (OEIS A016269)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_int_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= (seq_t)base;
    return res;
}

seq_t a(idx_t n)
{
    /* 2^(n+1) and 2^(n+2) */
    seq_t pow2_nplus1 = pow_int_seq(2, n + 1);
    seq_t pow2_nplus2 = pow_int_seq(2, n + 2);

    /* 3^(n+2) */
    seq_t pow3_nplus2 = pow_int_seq(3, n + 2);

    /* term1 = 2^(n+1) * (1 + 2^(n+2)) */
    seq_t term1 = pow2_nplus1 * ((seq_t)1 + pow2_nplus2);

    return term1 - pow3_nplus2;
}
