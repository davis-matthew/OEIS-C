/*
 * A064865  Fill a triangular array by rows by writing numbers 1, then 1 up to
 *          2^2, then 1 up to 3^2, then 1 up to 4^2 and so on. The final
 *          elements of the rows form the sequence.
 * https://oeis.org/A064865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = n*(n+1)/2 – max{p(m) | p(m) < n*(n+1)/2}, where p(m) =
 *            m*(m+1)*(2m+1)/6 is the m‑th square‑pyramidal number
 * Range:     u8 n <= 16, u16 n <= 247, u32 n <= 7445, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   squarepyr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/squarepyr.h"

seq_t a(idx_t n)
{
    seq_t T = ((seq_t)n * ((seq_t)n + 1)) / 2;   /* triangular number Tₙ */
    idx_t low  = 0;
    idx_t high = n;                               /* m < n for all n ≥ 1 */

    /* Binary search for the largest m with p(m) < T */
    while (low < high)
    {
        idx_t mid = low + (high - low + 1) / 2;   /* upper mid for right choice */
        if (squarepyr(mid) < T)
            low = mid;
        else
            high = mid - 1;
    }

    return T - squarepyr(low);
}
