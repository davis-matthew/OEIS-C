/*
 * A175567  a(n) = (n!)^2 modulo n*(n+1)/2.
 * https://oeis.org/A175567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (n!)^2 mod n*(n+1)/2
 * Range:     u8 n <= 26, u16 n <= 364, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) = (n!)^2 modulo n*(n+1)/2. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, behaviour for n=0 irrelevant */

    /* Compute the modulus T(n) = n*(n+1)/2 using seq_t arithmetic. */
    seq_t mod = ((seq_t)n * ((seq_t)n + 1)) / 2;

    /* Compute n! modulo mod.  The product never exceeds (mod * n),
       which is at most about 5·10^12 for n <= 10000, fitting in 64 bits. */
    seq_t cur = 1;
    for (idx_t i = 2; i <= n; ++i) {
        unsigned long long prod = (unsigned long long)cur * (unsigned long long)i;
        cur = (seq_t)(prod % (unsigned long long)mod);
    }

    /* Square the result modulo mod. */
    unsigned long long sq = (unsigned long long)cur * (unsigned long long)cur;
    return (seq_t)(sq % (unsigned long long)mod);
}
