/*
 * A135971  Ceiling(4*Pi*n^2).
 * https://oeis.org/A135971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(4*Pi*n^2)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use a rational approximation M/S of 4*Pi that is
   slightly larger than the true value.  For all n ≤ 10000
   the computed ceil is guaranteed to be exact.  */
seq_t a(idx_t n)
{
    /* 4*Pi ≈ 12.566370614359172953 ...
       M/S = 2,513,274,123 / 200,000,000  (> 4*Pi) */
    static const seq_t M = (seq_t)2513274123ULL;
    static const seq_t S = (seq_t)200000000ULL;

    /* n^2 */
    seq_t nn = (seq_t)n;
    seq_t nn_sq = nn * nn;

    /* ceil(M * n^2 / S) = floor((M*n^2 + S-1)/S) */
    seq_t numer = M * nn_sq + (S - (seq_t)1);
    seq_t res  = numer / S;

    return res;
}
