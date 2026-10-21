/*
 * A204437  Symmetric matrix: f(i,j)=((i+j+1)^2 mod 3), by (constant)
 *          antidiagonals.
 * https://oeis.org/A204437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((⌈(1+√(1+8 n))/2⌉ + 1)^2 mod 3)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#pragma once

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* Find the smallest s with (s-1)*s/2 >= n */
    idx_t low = 1, high = n + 1;            /* s <= n+1 is always true */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* Use 128‑bit arithmetic to avoid overflow */
        __int128 val = (__int128)(mid - 1) * mid / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low; /* antidiagonal sum (i + j) */

    /* a(n) = ((s+1)^2 mod 3), but (s+1) mod 3 is enough */
    idx_t mod3 = (s + 1) % 3;
    seq_t result = (seq_t)((mod3 * mod3) % 3);
    return result;
}
