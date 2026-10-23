/*
 * A174411  Triangle read by rows: T(n,k) = c(n) / c(max(k, n-k)) where c(n) =
 *          Product_{i=1..n} F(i)*F(i+1) and F(i) the Fibonacci numbers.
 * https://oeis.org/A174411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) is obtained by flattening the triangle T(n,k) where T(n,k)
 *            = c(n) / c(max(k,n-k)) and c(n) = ∏_{i=1..n} F(i)*F(i+1). F is
 *            the Fibonacci sequence. Indexing starts at n = 0
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline __int128 offset_row(idx_t r)
{
    return (__int128)r * (r + 1) / 2;
}

static inline idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    while (offset_row(high) <= (__int128)n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (offset_row(mid) <= (__int128)n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    idx_t r     = find_row(n);
    idx_t offset = (idx_t)offset_row(r);
    idx_t k     = n - offset;
    idx_t m     = (k > (r - k)) ? k : (r - k);

    seq_t res = (seq_t)1;
    if (m < r) {
        seq_t f_prev = (seq_t)1; /* F(1) */
        seq_t f_curr = (seq_t)1; /* F(2) */
        for (idx_t i = 1; i <= r; ++i) {
            if (i >= m + 1 && i <= r) {
                seq_t term = f_prev * f_curr;
                res = res * term;
            }
            seq_t f_next = f_prev + f_curr;
            f_prev = f_curr;
            f_curr = f_next;
        }
    }
    return res;
}
