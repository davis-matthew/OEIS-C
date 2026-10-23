/*
 * A154681  Triangle read by rows where T(m,n) = 2*m*n + m + n +3.
 * https://oeis.org/A154681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2*m*k + m + k + 3 where m is the smallest integer with
 *            m*(m+1)/2 ≥ n and k = n - (m-1)*m/2. This reads the triangular
 *            array T(m,k) = 2*m*k + m + k + 3 row‑by‑row
 * Range:     u8 n <= 65, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset starts at 1; behaviour below 1 is irrelevant */
    idx_t m = find_row(n);
    unsigned __int128 prev = (unsigned __int128)(m - 1) * m / 2;
    idx_t k = n - (idx_t)prev;     /* 1 ≤ k ≤ m */
    unsigned __int128 val = 2ULL *
                            (unsigned __int128)m * k +
                            (unsigned __int128)m +
                            (unsigned __int128)k + 3;
    return (seq_t)val;
}
