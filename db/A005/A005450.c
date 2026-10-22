/*
 * A005450  Numerator of (1 + Gamma(n))/n.
 * https://oeis.org/A005450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator(((n-1)! + 1)/n). By Wilson's theorem
 *            gcd((n-1)!+1, n) = n if n is prime and 1 otherwise, so a(n) =
 *            ((n-1)!+1)/n for prime n, else (n-1)!+1
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   a005450_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/a005450_isprime.h"

seq_t a(idx_t n) {
    if (!a005450_isprime(n)) {
        seq_t f = 1;
        for (idx_t i = 2; i < n; i++) f *= (seq_t)i;
        return f + 1;
    }
    /* n prime: track k! = q*n + r with 0 <= r < n, avoiding forming (n-1)! itself */
    seq_t q = 0;
    unsigned __int128 r = 1;
    for (idx_t k = 2; k < n; k++) {
        unsigned __int128 t = r * (unsigned __int128)k;
        q *= (seq_t)k;
        q += (seq_t)(uint64_t)(t / n);
        r = t % n;
    }
    /* r == n-1 by Wilson, so (r+1)/n == 1 */
    q += (seq_t)(uint64_t)((r + 1) / n);
    return q;
}
