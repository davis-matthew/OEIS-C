/*
 * A020648  Least positive integer k for which 9^n divides k!.
 * https://oeis.org/A020648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that 9^n divides k!. Criterion: v_3(k!)
 *            >= 2n, where v_3(m) is the exponent of 3 in m
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Upper bound: k will be less than 6n for all n >= 1. */
    idx_t low  = 1;
    idx_t high = n * 6 + 10;  /* safe buffer */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;

        /* Compute exponent of 3 in mid! */
        seq_t exp3 = 0;
        for (idx_t p = 3; p <= mid; ) {
            exp3 += mid / p;
            if (p > mid / 3) break;   /* avoid overflow of p *= 3 */
            p *= 3;
        }

        if (exp3 >= (seq_t)n * 2) {
            high = mid;      /* mid is a candidate, search smaller */
        } else {
            low = mid + 1;   /* need larger k */
        }
    }

    return (seq_t)low;
}
