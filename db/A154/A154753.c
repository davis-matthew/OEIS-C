/*
 * A154753  Reduced period of the Fibonacci 3-step sequence A000073 mod
 *          prime(n).
 * https://oeis.org/A154753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046738(prime(n)) – reduced period of the tribonacci *
 *            sequence modulo the nth prime. The tribonacci is defined by *
 *            t(1)=1, t(2)=1, t(3)=2 and t(k)=t(k-1)+t(k-2)+t(k-3). The period
 *            * r is the least index such that t(r-1)≡0 (mod p) and t(r)≡0
 *            (mod p)
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the nth prime (uint32_t) and cast to seq_t */
    seq_t p = (seq_t)get_prime(n);

    /* Initial tribonacci values modulo p */
    seq_t t1 = 1 % p;          /* t1 = t(n-3)   (t1 = 1)   */
    seq_t t2 = 1 % p;          /* t2 = t(n-2)   (t2 = 1)   */
    seq_t t3 = 2 % p;          /* t3 = t(n-1)   (t3 = 2)   */

    /* Check trivial early pairs – not needed but harmless */
    if (t1 == 0 && t2 == 0) return 2;  /* impossible for a prime >2 */
    if (t2 == 0 && t3 == 0) return 3;  /* impossible for a prime >2 */

    /* Iterate until we encounter two successive zeros */
    for (idx_t i = 4; ; ++i) {
        /* next = (t1 + t2 + t3) mod p, done with partial sums
         * to avoid overflow and without using larger integer types. */
        seq_t next = t1 + t2;
        if (next >= p) next -= p;
        next += t3;
        if (next >= p) next -= p;

        /* If previous value (t3) and next are both zero, we found the period. */
        if (t3 == 0 && next == 0) return i;

        /* Shift the window for the next iteration. */
        t1 = t2;
        t2 = t3;
        t3 = next;
    }
}
