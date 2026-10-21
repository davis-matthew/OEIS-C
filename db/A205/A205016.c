/*
 * A205016  Ordered differences of oblong numbers.
 * https://oeis.org/A205016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n≥1, let k be the smallest integer with k*(k+1)/2 ≥ n. Set j
 *            = k+1 and i = n – (k-1)*k/2. Then a(n) = j*(j+1) – i*(i+1) where
 *            s(m) = m*(m+1) is an oblong number
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* should not occur for the official offset */

    /* Binary search for minimal k such that k*(k+1)/2 ≥ n */
    idx_t low = 1, high = n, k = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n) {
            k = mid;
            if (mid == 0) break;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    /* Compute indices i and j */
    idx_t j = k + 1;
    idx_t n_before = (k - 1) * k / 2;   /* (k-1)*k/2 fits in idx_t for n up to 2^64-1 */
    idx_t i = n - n_before;

    /* Compute the difference s(j) - s(i) = j*(j+1) - i*(i+1) */
    unsigned __int128 term_j = (unsigned __int128)j * (j + 1);
    unsigned __int128 term_i = (unsigned __int128)i * (i + 1);
    return (seq_t)(term_j - term_i);
}
