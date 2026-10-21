/*
 * A220863  Choose smallest m>0 such that the n-th rational prime p splits in
 *          the imaginary quadratic extension field K = Q(sqrt(-m)); a(n) =
 *          discriminant(K).
 * https://oeis.org/A220863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the discriminant of Q(√(-m)) where m is the smallest
 *            positive integer such that the n‑th rational prime p splits in
 *            this imaginary quadratic field. For odd p the condition is
 *            (m/p)=(-1/p); for p=2 the smallest such m is 7
 * Range:     i8 n <= 31, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety guard, n>=1 in usage */

    uint64_t p = get_prime(n);

    /* Special case: prime 2 */
    if (p == 2) return -((seq_t)7);

    /* (-1/p) = 1 if p % 4 == 1 else -1 */
    int target = (p % 4 == 1) ? 1 : -1;

    uint64_t m = 1;
    for (;;) {
        if (m % p != 0) {                     /* m must not be divisible by p */
            seq_t e = mod_pow((seq_t)(m % p), (p-1)/2, (seq_t)p);
            int ls = (e == 1) ? 1
                    : (e == (seq_t)p - 1) ? -1 : 0;
            if (ls == target) {
                /* Construct discriminant */
                if (m % 4 == 3)
                    return -((seq_t)m);          /* D = -m when m ≡ 3 (mod 4) */
                else
                    return -((seq_t)(4 * m));    /* D = -4m otherwise */
            }
        }
        ++m;                                     /* try next integer */
    }
}
