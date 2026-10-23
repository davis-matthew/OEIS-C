/*
 * A048842  Least positive integer k for which 13^n divides k!.
 * https://oeis.org/A048842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k such that 13^n divides k!
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   exp13
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Legendre's formula + binary search */

#include <stdint.h>
#include "../utils/exp13.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;              /* 13^0 always divides 0! */

    seq_t target = (seq_t)n;   /* exponent of 13 that must appear in k! */

    seq_t low = 1;
    seq_t high = 13;           /* initial guess: certainly enough for n=1 */

    /* enlarge high until exp13(high) ≥ target */
    while (exp13(high) < target) {
        seq_t next = high << 1;
        if (next <= high || next == (seq_t)-1) {   /* overflow guard */
            high = (seq_t)-1;                      /* largest representable value */
            break;
        }
        high = next;
    }

    /* binary search for minimal k */
    while (low < high) {
        seq_t mid = low + ((high - low) >> 1);
        if (exp13(mid) >= target)
            high = mid;
        else
            low = mid + 1;
    }

    return low;
}
