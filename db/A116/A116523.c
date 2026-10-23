/*
 * A116523  a(0)=1, a(1)=1, a(n) = 17*a(n/2) for n=2,4,6,..., a(n) =
 *          16*a((n-1)/2) + a((n+1)/2) for n=3,5,7,....
 * https://oeis.org/A116523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} 16^wt(k) where wt(k) is the binary weight
 *            of k
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the nth term of OEIS A116523.
   a(0) = 0, a(1) = 1, ...  */
seq_t a(idx_t n)
{
    /* Pre‑compute powers of 16 up to 2^64 bits (maximum popcount for uint64_t). */
    static seq_t pow16[65];
    static int initialized = 0;
    if (!initialized) {
        pow16[0] = 1;
        for (int i = 1; i < 65; ++i)
            pow16[i] = pow16[i - 1] * ((seq_t)16);
        initialized = 1;
    }

    seq_t sum = 0;
    for (idx_t k = 0; k < n; ++k) {
        unsigned int pc = __builtin_popcountll((unsigned long long)k);
        sum += pow16[pc];
    }
    return sum;
}
