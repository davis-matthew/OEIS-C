/*
 * A183137  a(n) = [1/s] + [2/s] + ... + [n/s], where s = (golden ratio)^2 =
 *          (3+sqrt(5))/2 and [] = floor.
 * https://oeis.org/A183137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [1/s] + [2/s] + ... + [n/s] with s = (3+sqrt(5))/2 (the
 *            golden ratio squared)
 * Range:     u8 n <= 37, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Author: ChatGPT */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool can_inc(idx_t k, idx_t m) {
    /* Returns true iff k >= (m+1)*s, where s = (3+sqrt(5))/2.
       Uses the algebraic identity: 2k >= (m+1)(3+sqrt5)
       <=> 2k - 3(m+1) >= (m+1)*sqrt5.
       Squares both sides: (2k-3(m+1))^2 >= 5 (m+1)^2. */
    __int128 L = (__int128)2 * k - (__int128)3 * (m + 1);
    if (L <= 0) return false;
    __int128 left  = L * L;
    __int128 right = (__int128)5 * (m + 1) * (m + 1);
    return left >= right;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t m = 0; /* floor(k/s) for current k */

    for (idx_t k = 1; k <= n; ++k) {
        /* Increment m while k >= (m+1)*s. */
        while (can_inc(k, m)) ++m;
        sum += (seq_t)m;
    }

    return sum;
}
