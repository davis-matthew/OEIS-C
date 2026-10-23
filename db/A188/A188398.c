/*
 * A188398  a(n) = [n*r+k*r] - [n*r] - [k*r], where r=1/sqrt(2), k=5, [
 *          ]=floor.
 * https://oeis.org/A188398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊(n+5)/√2⌋ – ⌊n/√2⌋ – 3
 * Range:     u8 n <= 2, u16 n <= 26, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (This is equivalent to a(n) = ⌊n*r+5*r⌋ – ⌊n*r⌋ – ⌊5*r⌋ with r = 1/√2) */
#include <stdint.h>

static seq_t int_sqrt(seq_t x)
/* Integer square root: returns ⌊√x⌋ using binary search.  */
{
    seq_t low = 0;
    seq_t high = x;
    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        seq_t mid2 = mid * mid;
        if (mid2 <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

static seq_t floor_sqrt2_div(seq_t n)
/* Compute ⌊ n / √2 ⌋ without using floating‑point arithmetic.  */
{
    seq_t prod = n * n;          /* n² (assumed to fit in seq_t)       */
    prod /= (seq_t)2;            /* n² / 2 */
    return int_sqrt(prod);       /* ⌊√(n²/2)⌋ = ⌊n/√2⌋                  */
}

seq_t a(idx_t n)
/* Main sequence function */
{
    seq_t nn = (seq_t)n;
    seq_t f0 = floor_sqrt2_div(nn);       /* ⌊n/√2⌋   */
    seq_t f1 = floor_sqrt2_div(nn + 5);   /* ⌊(n+5)/√2⌋ */
    return f1 - f0 - (seq_t)3;
}
