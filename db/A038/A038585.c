/*
 * A038585  Write n in binary, delete 0's.
 * https://oeis.org/A038585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   a(n) = (10^{popcount(n)}-1)/9 – a(n) is the decimal number
 *            consisting of popcount(n) ones (write n in binary, delete all
 *            zeros)
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 896 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Count the number of 1‑bits in an unsigned 64‑bit integer.
   A simple portable implementation that works for any value of idx_t. */
static inline idx_t popcount_idx(idx_t val)
{
    idx_t count = 0;
    while (val) {
        val &= val - 1;   /* clear the lowest set bit */
        ++count;
    }
    return count;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 for the series; its popcount is at least 1. */
    idx_t k = popcount_idx(n);            /* number of 1‑bits in n */
    seq_t pow = pow10(k);                 /* 10^k as a seq_t value */
    return (pow - 1) / 9;                 /* (10^k – 1) / 9 gives a string of k ones */
}
