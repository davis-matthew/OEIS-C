/*
 * A092370  Triangle read by rows: T(n,k)=(1/2)*C(n+k,k)*C(n,n-k).
 * https://oeis.org/A092370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (1/2) * C(r+k, k) * C(r, k) * where r is the row index
 *            such that r*(r+1)/2 >= n * (rows are numbered starting at 1) and
 *            k = n - r*(r-1)/2
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   binomial, floor_sqrt_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/floor_sqrt_uint64.h"

#ifdef __cplusplus
#include <type_traits>
#endif

static inline seq_t compute_T(idx_t r, idx_t k)
{
    seq_t a = binomial(r, k);          /* C(r, k) */
    seq_t b = binomial(r + k, k);      /* C(r+k, k) */

#ifndef __cplusplus
    unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)b;
    return (seq_t)(prod / 2);
#else
    if constexpr (std::is_integral_v<seq_t>) {
        unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)b;
        return (seq_t)(prod / 2);
    } else {
        seq_t prod = a * b;
        return prod / 2;
    }
#endif
}

seq_t a(idx_t n)
{
    /* total terms of rows 1 .. r-1 is r*(r-1)/2 */
    uint64_t nn = (uint64_t)n;
    uint64_t sq = floor_sqrt_uint64(1 + 8 * nn);
    uint64_t r = (sq - 1) / 2;
    if (r * (r + 1) / 2 < nn)    /* smallest r with r*(r+1)/2 ≥ n */
        ++r;
    uint64_t prev = r * (r - 1) / 2;   /* terms before row r */
    uint64_t k    = nn - prev;         /* 1‑based column within the row */

    return compute_T((idx_t)r, (idx_t)k);
}
