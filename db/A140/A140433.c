/*
 * A140433  Primes of the form n^0+(n+1)^1+(n+2)^2+(n+3)^3+(n+4)^4.
 * https://oeis.org/A140433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 6, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form n^0+(n+1)^1+(n+2)^2+(n+3)^3+(n+4)^4.
   a(n) is the nth such prime in ascending order of n. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* not part of OEIS offset, but harmless */
    idx_t m = 1;            /* current n in the definition                 */
    idx_t primes_found = 0;
    seq_t sum;

    while (true) {
        /* compute n^0 + (n+1)^1 + (n+2)^2 + (n+3)^3 + (n+4)^4 */
        sum = 1;                                 /* n^0 = 1                */
        seq_t base = (seq_t)m + 1;               /* n+1                    */
        sum += base;                             /* (n+1)^1                */

        base = (seq_t)m + 2;                     /* n+2                    */
        seq_t t = base * base;                   /* (n+2)^2                */
        sum += t;

        base = (seq_t)m + 3;                     /* n+3                    */
        t = base * base;                         /* (n+3)^2                */
        t = t * base;                            /* (n+3)^3                */
        sum += t;

        base = (seq_t)m + 4;                     /* n+4                    */
        t = base * base;                         /* (n+4)^2                */
        t = t * t;                               /* (n+4)^4                */
        sum += t;

        if (is_prime_big(sum)) {                 /* keep only primes       */
            primes_found++;
            if (primes_found == n) {
                return sum;                     /* nth prime sum found    */
            }
        }
        m++;
    }
}
