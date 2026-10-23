/*
 * A014772  Squares of even hexagonal numbers.
 * https://oeis.org/A014772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(4*n-1))^2 (Squares of even hexagonal numbers;
 *            H_{2n} = (2n)*(4n-1))
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to seq_t to avoid intermediate overflows */
    seq_t x = (seq_t)n;

    /* compute 2*n and 4*n-1 */
    seq_t two_n   = 2 * x;      /* 2n          */
    seq_t four_n  = 4 * x;      /* 4n          */
    seq_t four_n_minus_one = four_n - 1; /* 4n-1       */

    /* product 2n*(4n-1) */
    seq_t prod = two_n * four_n_minus_one;

    /* square the product */
    return prod * prod;
}
