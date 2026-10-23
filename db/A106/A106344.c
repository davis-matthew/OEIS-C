/*
 * A106344  Triangle read by rows: T(n,k) = binomial(k,n-k) mod 2.
 * https://oeis.org/A106344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(k, r-k) mod 2, where r = floor((√(8n+1)-1)/2)
 *            and k = n - r(r+1)/2. The triangle is read row‑by‑row with rows
 *            consisting of k=0…r
 * Range:     u8 n <= 1274, u16 n <= 1274, u32 n <= 1274, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.  The search
   uses binary expansion followed by binary search. */
static inline idx_t row_of_index(idx_t n)
{
    idx_t low = 0, high = 1;
    while ((__int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);
    __int128 start = (__int128)r * (r + 1) / 2;
    idx_t k = (idx_t)(n - start);
    idx_t a_val = r - k;   /* r-k */
    idx_t b_val = k;       /* k   */
    /* Lucas' theorem for mod 2: binomial(a,b) is odd iff (a & ~b)==0. */
    return ((a_val & ~b_val) == 0) ? 1 : 0;
}
