/*
 * A127185  Triangle of distances between n>=1 and n>=m>=1 measured by the
 *          number of non-common prime factors.
 * https://oeis.org/A127185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl, changed
 * Formula:   a(n) = T(r,c) where r,c are the row and column of the n‑th entry
 *            in the triangle read by rows. T(r,c) = ω(r/g) + ω(c/g) with g =
 *            gcd(r,c) ω(x) is the big–Omega function (number of prime factors
 *            of x, counted with multiplicity). This implements the OEIS
 *            sequence A127185
 * Range:     u8 n <= 9460, u16 n <= 9460, u32 n <= 9460, u64 n <= 9460,
 *            u128 n <= 9460, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_uint64.h"

/* Count prime factors of x (with multiplicity) – big–Omega. */
static inline uint64_t bigomega_uint64(uint64_t x)
{
    uint64_t cnt = 0;
    while ((x & 1ULL) == 0) {  /* divisible by 2 */
        ++cnt;
        x >>= 1;
    }
    for (uint64_t p = 3; p * p <= x; p += 2) {
        while (x % p == 0) {
            ++cnt;
            x /= p;
        }
    }
    if (x > 1) ++cnt;          /* remainder is prime */
    return cnt;
}

/* Find the row r such that r*(r+1)/2 >= n and (r-1)*r/2 < n.
   Uses 128-bit arithmetic to avoid overflow. */
static inline uint64_t row_of_index(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t low = 1, high = 2;
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;           /* exponential search */
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n) high = mid;
        else          low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    uint64_t r = row_of_index(n);
    unsigned __int128 prev_tri_val = (unsigned __int128)(r - 1) * r / 2;
    uint64_t prev_tri = (uint64_t)prev_tri_val;
    uint64_t c = n - prev_tri;          /* column index, 1‑based */

    uint64_t g = gcd_uint64(r, c);

    uint64_t part_r = r / g;
    uint64_t part_c = c / g;

    uint64_t omega_r = bigomega_uint64(part_r);
    uint64_t omega_c = bigomega_uint64(part_c);

    return (seq_t)(omega_r + omega_c);
}
