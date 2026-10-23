/*
 * A136214  Triangle U, read by rows, where U(n,k) = Product_{j=k..n-1}
 *          (3*j+1) for n > k with U(n,n) = 1.
 * https://oeis.org/A136214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the element read by rows of triangle U(r,k) =
 *            prod_{j=k}^{r-1} (3*j+1), with U(r,r)=1. The row r containing
 *            index n satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n -
 *            r*(r+1)/2
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A136214. */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Compute shift k within the row */
    unsigned __int128 before128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t before = (idx_t)before128;
    idx_t k = n - before;

    /* Compute U(r,k) = product_{j=k}^{r-1} (3*j+1) */
    seq_t result = 1;
    for (idx_t j = k; j < r; ++j) {
        seq_t factor = ((seq_t)j) * (seq_t)3 + 1;
        result *= factor;
    }
    return result;
}
