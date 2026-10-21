/*
 * A096398  Nonnegative numbers k such that 0 = #{ 0 <= i <= k : K(k, i) = -1
 *          } where K(k, i) is the Kronecker symbol.
 * https://oeis.org/A096398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A096398: squares together with the exceptional values 2 and 6.
   For n = 1..5 the values are listed explicitly; otherwise
   a(n) = (n-3)^2.  This follows from the fact that every
   non‑zero member of the sequence is either a perfect square or
   one of the two special nonsquares 2,6. */
seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly */
    static const seq_t small[5] = {0, 1, 2, 4, 6};
    if (n <= 5)
        return small[n - 1];

    /* For n >= 6 the sequence consists only of squares.
       The n-th term then satisfies a(n) = (n-3)^2. */
    seq_t m = (seq_t)(n - 3);
    return m * m;
}
