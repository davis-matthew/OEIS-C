/*
 * A168256  Triangle read by rows: Catalan number C(n) repeated n+1 times.
 * https://oeis.org/A168256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = Catalan( floor((sqrt(8*n+1)-1)/2) )
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row index k such that k*(k+1)/2 <= n < (k+1)*(k+2)/2. */
    idx_t low = 0, high = n;          /* k is always <= n */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;          /* upper mid */
        seq_t t = ((seq_t)mid * (mid + 1)) / 2;
        if (t <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t k = low;  /* row index */

    /* Compute binomial(2k, k) with a multiplicative formula.
       The division is exact at every step. */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = (res * (k + i)) / i;

    /* Catalan number C(k) = binom(2k, k)/(k+1). */
    return res / (k + 1);
}
