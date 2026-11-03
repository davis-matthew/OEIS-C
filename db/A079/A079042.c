/*
 * A079042  Numbers n in which the first k digits of n form an integer
 *          divisible by k^2, for k = 1, 2, ..., m, where m is the number of
 *          digits in n.
 * https://oeis.org/A079042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: full, pass, 61 terms through n = 61 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A079042.  
   A(n) is the nth positive integer such that for every k = 1..m
   (m = number of decimal digits of the integer), the first k digits
   form a number divisible by k². */

#include <stdint.h>
#include <stdbool.h>

static bool is_good(seq_t x)
{
    /* Count decimal digits of x. */
    int m = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        tmp /= 10;
        ++m;
    }
    if (m == 0) return false;  /* exclude 0 */

    seq_t divisor = 1;
    for (int k = m; k >= 1; --k) {
        seq_t prefix = x / divisor;

        /* Compute k² as an unsigned 64‑bit integer, then cast to seq_t. */
        uint64_t k_sq = (uint64_t)k * (uint64_t)k;
        seq_t sq = (seq_t)k_sq;

        if (prefix % sq != 0) return false;
        divisor *= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, but harmless */

    seq_t candidate = 0;
    idx_t found = 0;

    while (found < n) {
        ++candidate;
        if (is_good(candidate)) ++found;
    }

    return candidate;
}
