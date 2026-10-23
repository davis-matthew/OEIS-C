/*
 * A177990  Triangle read by rows, variant of A070909, a cellular automaton.
 * https://oeis.org/A177990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 (for n=1); otherwise, let r be the row such that T(r-1)
 *            < n <= T(r) (T(m)=m(m+1)/2) and let c=n-T(r-1). Then a(n) = 0 if
 *            c==1; otherwise a(n) = 1 if (c even) or (c==r); otherwise a(n) =
 *            0
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1u) return (seq_t)1u;

    /* Find row r: smallest r with r*(r+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1u) / 2u;
        if (t >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;

    /* column index within the row */
    unsigned __int128 base = (unsigned __int128)r * (r - 1u) / 2u;
    idx_t c = (idx_t)((unsigned __int128)n - base);

    if (c == 1u) return (seq_t)0u;
    if ((c & 1u) == 0u) return (seq_t)1u;
    if (c == r)         return (seq_t)1u;
    return (seq_t)0u;
}
