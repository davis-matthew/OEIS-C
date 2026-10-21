/*
 * A072872  a(n) is the smallest positive number k such that n divides 2^k -
 *          k.
 * https://oeis.org/A072872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n)  =  smallest positive k such that n divides 2^k - k  */
/*  Computation: iterate k increasing, maintaining 2^k mod n */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety, though n>=1 in use */
    seq_t mod = (seq_t)n;            /* use seq_t for modulo operations */
    seq_t val = 2 % mod;             /* 2^1 mod n */

    for (idx_t k = 1; ; ++k) {
        if (val == (seq_t)(k % n))
            return (seq_t)k;        /* found smallest k */

        /* advance to 2^(k+1) mod n */
        val = (val * 2) % mod;
    }
}
