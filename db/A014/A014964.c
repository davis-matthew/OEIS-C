/*
 * A014964  a(n) = lcm(n, 2^(n-1)).
 * https://oeis.org/A014964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, 2^(n-1))
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 28, u64 n <= 60, u128 n <= 122,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/gcd.h"

/* Computes a(n) = lcm(n, 2^(n-1)) for n >= 1.
 * The function accepts the index as an unsigned 64‑bit integer (idx_t) and
 * returns the sequence value as seq_t, which may be any unsigned integer
 * type or an arbitrary‑precision integer class.  The implementation
 * uses the provided helpers pow2 and gcd to keep the computation
 * simple and fast.
 */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, return 0 as guard */

    seq_t nseq = (seq_t)n;          /* convert index to seq_t */
    seq_t two_pow = pow2(n - 1);    /* 2^(n-1) */
    seq_t g = gcd(nseq, two_pow);   /* gcd(n, 2^(n-1)) = 2^v2(n) */
    seq_t result = nseq / g;        /* reduce n before multiplying by the power of two */
    result *= two_pow;
    return result;
}
