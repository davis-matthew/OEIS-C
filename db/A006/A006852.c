/*
 * A006852  Step at which n is expelled in Kimberling's puzzle (A035486).
 * https://oeis.org/A006852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   simulation of Kimberling's expulsion: track the position p of n,
 *            starting at p = n. At step i: if p == i, n is expelled and a(n)
 *            = i; if i < p < 2i then p -> 2(p-i)-1; if p < i then p ->
 *            2(i-p); if p >= 2i then p -> p-1
 * Range:     u8 n <= 18, u16 n <= 39, u32 n <= 3329, u64 n <= 9968,
 *            u128 n <= 9968, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t p = (uint64_t)n;
    uint64_t i;
    for (i = 1;; i++) {
        if (p == i)
            return (seq_t)i;
        if (p > i) {
            if (p < 2 * i)
                p = 2 * (p - i) - 1;
            else
                p = p - 1;
        } else {
            p = 2 * (i - p);
        }
    }
}
