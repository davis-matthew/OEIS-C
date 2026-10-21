/*
 * A232597  a(n) = sum of odd k in 1..n for which Kronecker(k,n)=1.
 * https://oeis.org/A232597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{odd k≤n and Kronecker(k,n)=1} k
 * Range:     u8 n <= 45, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   jacobi64, kronecker64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/jacobi64.h"
#include "../utils/kronecker64.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; k += 2) {
        if (kronecker64(k, n) == 1)
            sum += (seq_t)k;
    }
    return sum;
}
