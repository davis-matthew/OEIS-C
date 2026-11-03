/*
 * A023965  First digit after decimal point of 6th root of n.
 * https://oeis.org/A023965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(10*n^(1/6)) mod 10
 * Range:     u8 none, u16 none, u32 n <= 1001, u64 n <= 1001, u128 n <= 1001,
 *            big any n
 * Validated: standard, pass, 118 terms through n = 945 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute v^6, where v fits in idx_t. */
static inline seq_t power6_idx(idx_t v)
{
    seq_t p = (seq_t)v;
    p *= (seq_t)v; /* v^2 */
    p *= (seq_t)v; /* v^3 */
    p *= (seq_t)v; /* v^4 */
    p *= (seq_t)v; /* v^5 */
    p *= (seq_t)v; /* v^6 */
    return p;
}

/* Integer sixth root: floor of x^(1/6). */
static inline idx_t integer_sixth_root(seq_t x)
{
    /* Find an upper bound. */
    idx_t low = 0, high = 1;
    while (1) {
        seq_t pw = power6_idx(high);
        if (pw <= x) {
            low = high;
            high <<= 1; /* doubling the bound */
        } else {
            break;
        }
    }
    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        seq_t mid6 = power6_idx(mid);
        if (mid6 <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Scale by 10^6 to capture the first decimal digit. */
    seq_t scaled = (seq_t)n * 1000000ULL;
    idx_t root = integer_sixth_root(scaled);
    return (seq_t)(root % 10);
}
