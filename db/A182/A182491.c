/*
 * A182491  T(n,k), a triangular array read by rows, is the Wiener index for
 *          the complete bipartite graph K(n,k).
 * https://oeis.org/A182491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = r*k + r*(r-1) + k*(k-1), where r = the unique integer
 *            with (r-1)*r/2 < n <= r*(r+1)/2 and k = n - (r-1)*r/2. This
 *            yields the OEIS sequence A182491
 * Range:     u8 n <= 54, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* n < 1 is out of bounds; return 0 */

    idx_t low = 1, high = n;          /* r is between 1 and n inclusive */

    /* Binary search for the smallest r such that r*(r+1)/2 >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* k = position within row r */
    unsigned __int128 before = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)before;

    /* Convert to seq_t and compute T(r,k) */
    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;

    seq_t val = sr * sk + sr * (sr - 1) + sk * (sk - 1);
    return val;
}
