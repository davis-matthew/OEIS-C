/*
 * A116524  a(0)=1, a(1)=1, a(n) = 13*a(n/2) for n=2,4,6,..., a(n) =
 *          12*a((n-1)/2) + a((n+1)/2) for n=3,5,7,....
 * https://oeis.org/A116524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{k=0..n-1} 12^wt(k), where wt(k) is the number of
 *            1-bits in the binary expansion of k
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 507, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
 
/* Count set bits in a 64‑bit integer. */
static unsigned popcount_uint64(idx_t x)
{
    unsigned c = 0;
    while (x) {
        c += x & 1;
        x >>= 1;
    }
    return c;
}
 
seq_t a(idx_t n)
{
    /* Precompute powers of 12 up to 12^64 once. */
    static bool initialized = false;
    static seq_t pow12[65];
    if (!initialized) {
        pow12[0] = (seq_t)1;
        for (unsigned i = 1; i < 65; ++i) {
            pow12[i] = pow12[i - 1] * (seq_t)12;
        }
        initialized = true;
    }
 
    seq_t sum = 0;
    for (idx_t k = 0; k < n; ++k) {
        unsigned pc = popcount_uint64(k);
        sum += pow12[pc];
    }
    return sum;
}
