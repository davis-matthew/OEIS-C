/*
 * A232597  a(n) = sum of odd k in 1..n for which Kronecker(k,n)=1.
 * https://oeis.org/A232597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{odd k≤n and Kronecker(k,n)=1} k
 * Range:     u8 n <= 45, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   jacobi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/jacobi.h"

/* Kronecker symbol (a/n) for odd a and n≥1 */
static int kronecker64(uint64_t a, uint64_t n)
{
    if (n == 0) return 0;          /* not needed for the sequence       */
    int res = 1;
    /* factor powers of 2 from n */
    int e = __builtin_ctzll(n);    /* number of trailing zeros of n      */
    if (e & 1) {                   /* only odd power of 2 matters        */
        uint8_t r = a & 7;         /* a mod 8, a is odd                  */
        res = (r == 1 || r == 7) ? 1 : -1;
    }
    n >>= e;                       /* now n is odd                       */
    if (n == 1) return res;        /* only 2‑part remains               */
    int j = jacobi(a, n);
    if (j == 0) return 0;
    return (res * j > 0) ? 1 : -1;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; k += 2) {
        if (kronecker64(k, n) == 1)
            sum += (seq_t)k;
    }
    return sum;
}
