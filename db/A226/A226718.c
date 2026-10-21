/*
 * A226718  n! mod tetrahedral(n), that is A000142(n) mod A000292(n).
 * https://oeis.org/A226718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! mod (n*(n+1)*(n+2)/6); For n>4, if neither n+1 nor n+2
 *            is prime, a(n)=0; Otherwise a(n)= n*(n+1)/2 for odd n, a(n)=
 *            n*(n+2) for even n
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Simple primality test for idx_t values (valid for n up to about 1e7) */
static bool is_prime_idx(idx_t x) {
    if (x <= 1) return false;
    if (x <= 3) return true;
    if ((x & 1) == 0) return (x == 2);
    for (idx_t i = 3; i <= x / i; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    /* Handle the small n (1..4) explicitly */
    if (n <= 4) {
        switch ((unsigned)n) {
            case 1: return 0;   /* 1! mod 1 = 0 */
            case 2: return 2;   /* 2! mod 4 = 2 */
            case 3: return 6;   /* 6 mod 10 = 6 */
            case 4: return 4;   /* 24 mod 20 = 4 */
        }
    }

    /* For n > 4, use the derived formula */
    if (!is_prime_idx(n + 1) && !is_prime_idx(n + 2)) {
        return 0;
    }

    /* Compute the result according to parity of n */
    if (n & 1) {
        /* odd n: n*(n+1)/2 */
        return ((seq_t)n * ((seq_t)n + 1)) / 2;
    } else {
        /* even n: n*(n+2) */
        return (seq_t)n * ((seq_t)n + 2);
    }
}
