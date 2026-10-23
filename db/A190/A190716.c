/*
 * A190716  a(2*n) = 2*n and a(2*n-1) = A054569(n).
 * https://oeis.org/A190716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = 2*n ; a(2*n-1) = 4*n^2 - 6*n + 3
 * Range:     u8 n <= 16, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the sequence integer type once. */
    seq_t n_seq = (seq_t)n;

    /* Even indices satisfy a(n) = n. */
    if ((n & 1ULL) == 0) {
        return n_seq;
    }

    /* For odd indices let k = (n+1)/2;  a(n) = 4*k^2 - 6*k + 3. */
    seq_t k = (n_seq + 1) / 2;
    seq_t k_sq = k * k;

    /* Compute the polynomial with signed intermediate arithmetic
       casted to seq_t.  All operations are on unsigned values, so no
       overflow concerns for the ranges tested in the problem. */
    return 4 * k_sq - 6 * k + 3;
}
