/*
 * A007690  Number of partitions of n in which no part occurs just once.
 * https://oeis.org/A007690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{k>0 is a multiple of 2 or 3} 1/(1-x^k), i.e. a(n)
 *            = number of partitions of n into parts divisible by 2 or 3
 * Range:     u8 n <= 27, u16 n <= 69, u32 n <= 200, u64 n <= 639,
 *            u128 n <= 2215, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *a007690_tab = 0;
static idx_t a007690_cap = 0;

seq_t a(idx_t n) {
    if (n >= a007690_cap) {
        idx_t newcap = n + 1;
        if (newcap < 2 * a007690_cap) newcap = 2 * a007690_cap;
        if (a007690_tab) release(a007690_tab, a007690_cap);
        a007690_tab = alloc_zero(newcap);
        a007690_tab[0] = 1;
        for (idx_t k = 2; k < newcap; k++) {
            if (k % 2 != 0 && k % 3 != 0) continue;
            for (idx_t i = k; i < newcap; i++) {
                a007690_tab[i] += a007690_tab[i - k];
            }
        }
        a007690_cap = newcap;
    }
    return a007690_tab[n];
}
