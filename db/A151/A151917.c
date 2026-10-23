/*
 * A151917  a(0)=0, a(1)=1; for n>=2, a(n) = (2/3)*(Sum_{i=1..n-1} 3^wt(i)) +
 *          1, where wt() = A000120().
 * https://oeis.org/A151917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2/3)*(∑_{i=1}^{n-1} 3^wt(i)) + 1, where wt(i) is the
 *            number of 1 bits in the binary representation of i. Initial
 *            values: a(0)=0, a(1)=1. For n≥2 the formula holds
 * Range:     u8 n <= 16, u16 n <= 374, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */
#include <stddef.h>  /* for size_t */

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Pre‑compute powers of 3 up to 2^64 bits (maximum popcount).  */
    static seq_t pow3[65];
    static int pow3_init = 0;
    if (!pow3_init) {
        pow3[0] = (seq_t)1;
        for (int k = 1; k < 65; ++k) {
            pow3[k] = pow3[k - 1] * (seq_t)3;
        }
        pow3_init = 1;
    }

    /* Sum 3^popcount(i) for i = 1 .. n‑1.  */
    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i < n; ++i) {
        unsigned int pc = __builtin_popcountll((uint64_t)i);
        sum += pow3[pc];
    }

    /* sum is always a multiple of 3, so division is exact.  */
    seq_t half = sum / (seq_t)3;
    seq_t result = (seq_t)1 + half * (seq_t)2;
    return result;
}
