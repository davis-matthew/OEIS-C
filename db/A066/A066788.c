/*
 * A066788  a(n) = gcd(phi(n), n^2 + 1).
 * https://oeis.org/A066788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 307, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066788:  a(n) = gcd(phi(n), n^2 + 1)                 */
/* The function uses the public phi() routine and an inline Euclidean */
/* GCD algorithm that works for any seq_t.                          */

#include <stdint.h>
#include "../utils/phi.h"

/* Inline Euclidean algorithm for seq_t values.                     */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* φ(n) is defined for all n ≥ 1.  The public geminator phi()   */
    /* returns the totient as a value that fits in seq_t for the    */
    /* test range.                                                 */
    seq_t phi_n = phi(n);

    /* Compute (n^2 + 1) modulo φ(n).  n is at most 10 000 in our      */
    /* tests, so n^2+1 fits in a 64‑bit unsigned integer.            */
    uint64_t big   = (uint64_t)n * (uint64_t)n + 1ULL;
    seq_t  modval  = (seq_t)(big % (uint64_t)phi_n);

    /* gcd(φ(n), n^2+1) = gcd(φ(n), (n^2+1) mod φ(n))                 */
    return gcd_seq_t(phi_n, modval);
}
