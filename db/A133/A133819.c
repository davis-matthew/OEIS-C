/*
 * A133819  Triangle whose rows are sequences of increasing squares: 1; 1,4;
 *          1,4,9; ... .
 * https://oeis.org/A133819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = square of the rank of n within its triangle row. Row
 *            index t is the largest integer with t*(t+1)/2 < n. Then a(n) =
 *            (n - t*(t+1)/2)^2
 * Range:     u8 n <= 135, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t triangular(idx_t k)
{
    return (idx_t)((unsigned __int128)k * (k + 1) / 2);
}

static idx_t row_of(idx_t n)
{
    /* Find the largest t with triangular(t) < n by binary search. */
    idx_t low = 0;
    idx_t high = 1;
    while (triangular(high) < n) {
        if (high > (UINT64_MAX >> 1)) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) < n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t t = row_of(n);
    idx_t m = n - triangular(t);                 /* 1 <= m <= t+1 */
    seq_t m_val = (seq_t)m;
    return m_val * m_val;
}
