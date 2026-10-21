/*
 * A084756  For n, k > 0, let T(n, k) be given by T(n, 1) = n and T(n, k+1) =
 *          k*T(n, k) + 1. Then a(n) = T(n, n).
 * https://oeis.org/A084756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n,n) with * T(n,1) = n, * T(n,k+1) = k*T(n,k) + 1 * *
 *            The iterative evaluation below follows the recurrence directly
 *            and * requires only O(n) multiplications/additions. * Working in
 *            seq_t guarantees exactness when the result fits in the * chosen
 *            integer type; on 64‑bit builds seq_t is an...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset is 1; n=0 is not used */
    seq_t val = (seq_t)n;        /* T(n,1) */
    for (idx_t i = 1; i < n; ++i) {
        val = i * val + 1;          /* T(n,i+1) = i*T(n,i)+1 */
    }
    return val;
}
