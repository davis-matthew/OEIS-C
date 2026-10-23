/*
 * A112618  Let T(n) = A000073(n+1), n >= 1; a(n) = smallest k such that
 *          prime(n) divides T(k).
 * https://oeis.org/A112618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that prime(n) divides the tribonacci
 *            number T(k)
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, mod_add3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where T(1)=T(2)=1, T(3)=2, and T(i)=T(i-1)+T(i-2)+T(i-3) for i>3. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* provides seq_t get_prime(uint64_t idx) */
#include "../utils/mod_add3.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for indices < 1, but return 0 */

    /* Retrieve the nth prime (1‑indexed) */
    uint64_t p = (uint64_t)get_prime(n);

    /* Handle trivial primes (none for primes) */
    if (p <= 1)
        return (seq_t)1;

    /* Tribonacci numbers modulo p */
    uint64_t t1 = 1 % p;   /* T(1) */
    uint64_t t2 = 1 % p;   /* T(2) */
    uint64_t t3 = 2 % p;   /* T(3) */

    /* Check the first three terms */
    if (t1 == 0)   return (seq_t)1;
    if (t2 == 0)   return (seq_t)2;
    if (t3 == 0)   return (seq_t)3;

    idx_t k = 3;   /* current index for T(k) */

    for (;;) {
        uint64_t t4 = mod_add3(t1, t2, t3, p);   /* T(k+1) */
        ++k;
        if (t4 == 0)
            return (seq_t)k;   /* found minimal k */

        /* Shift for next iteration */
        t1 = t2;
        t2 = t3;
        t3 = t4;
    }
}
