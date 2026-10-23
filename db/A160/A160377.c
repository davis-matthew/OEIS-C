/*
 * A160377  Phi-torial of n (A001783) modulo n.
 * https://oeis.org/A160377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A160377: Phi‑torial of n modulo n
    a(n) = ∏_{k=1..n, gcd(k,n)=1} k  (mod n)
    (a(1)=0, a(2)=1, for n>2 a(n)=n‑1 iff n has a primitive root,
    otherwise a(n)=1) */

#include <stdbool.h>

static bool is_prime_power(idx_t n)
{
    if (n <= 1) return false;
    idx_t remaining = n;
    idx_t distinct = 0;
    idx_t p = 2;
    while (p * p <= remaining) {
        if (remaining % p == 0) {
            ++distinct;
            if (distinct > 1) return false;
            while (remaining % p == 0) remaining /= p;
        }
        if (p == 2) p = 3;
        else p += 2;
    }
    if (remaining > 1) ++distinct;
    return distinct == 1;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    bool prim_root = false;
    if (n == 2 || n == 4) {
        prim_root = true;
    } else if ((n & 1) == 0) {          /* n even, n>4   */
        idx_t m = n >> 1;              /* n/2           */
        if ((m & 1) != 0 && is_prime_power(m)) {
            prim_root = true;
        }
    } else {                            /* n odd         */
        if (is_prime_power(n)) prim_root = true;
    }

    return prim_root ? (seq_t)(n - 1) : (seq_t)1;
}
