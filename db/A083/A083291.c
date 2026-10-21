/*
 * A083291  Triangular array read by rows: T(n,k) = k*floor(n/10) + n mod 10,
 *          0<=k<=n.
 * https://oeis.org/A083291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (n - ⌊r(r+1)/2⌋) * ⌊r/10⌋ + (r mod 10), where r =
 *            ⌊(√(8n+1) - 1)/2⌋ (the largest integer with r(r+1)/2 ≤ n) (reads
 *            the triangular array T(n,k) = k·⌊n/10⌋ + n mod 10 by rows)
 * Range:     u8 n <= 1376, u16 n <= 8897, u32 n <= 8897, u64 n <= 8897,
 *            u128 n <= 8897, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 using binary search. */
    idx_t low  = 0;
    idx_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {    /* grow upper bound */
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    idx_t r = low - 1;
    idx_t k = n - (unsigned __int128)r * (r + 1) / 2;

    /* Compute T(r,k) = k · floor(r/10) + r mod 10. */
    seq_t val = ((seq_t)k) * ((seq_t)(r / 10)) + (seq_t)(r % 10);
    return val;
}
