/*
 * A038293  Triangle whose (i,j)-th entry is binomial(i,j)*9^(i-j)*3^j.
 * https://oeis.org/A038293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the (n‑th) entry of the triangle whose entries are *
 *            binomial(i,j) * 9^(i−j) * 3^j enumerated row‑wise (i from 0, j
 *            from 0..i)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>           /* for uint64_t */
#include "../utils/binomial.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Find i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2. */
    idx_t low  = 0;
    idx_t high = 1;

    /* Grow high until it exceeds the desired interval. */
    while (1) {
        unsigned __int128 tri = (unsigned __int128)high * (high + 1) / 2;
        if (tri > n) break;
        if (high >= (UINT64_MAX >> 1)) { high = UINT64_MAX; break; } /* avoid overflow */
        high <<= 1;
    }

    /* Binary search to refine the interval. */
    while (low < high) {
        idx_t mid = low + (high - low + 1)/2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) low = mid;
        else          high = mid - 1;
    }
    idx_t i = low;
    idx_t j = n - (unsigned __int128)i * (i + 1) / 2;

    /* Compute the term using the provided utilities. */
    seq_t val = binomial(i, j);

    /* 9^(i-j) = 3^(2*(i-j)) */
    idx_t exp9 = (i - j) * 2;
    val *= pow3(exp9);

    /* 3^j */
    val *= pow3(j);

    return val;
}
