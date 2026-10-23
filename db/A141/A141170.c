/*
 * A141170  Primes of the form x^2+4*x*y-2*y^2 (as well as of the form
 *          3*x^2+6*x*y+y^2).
 * https://oeis.org/A141170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1607, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A141170 – primes of the form x^2+4xy-2y^2
   (equivalently, primes ≡ 1 or 19 (mod 24), plus the prime 3). */
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, return 0 for safety */
    if (n == 1) return (seq_t)3;   /* first term */

    idx_t count = 1;               /* 3 counts as the first */
    idx_t candidate = 5;           /* start checking from 5 */

    for (;;) {
        if (is_prime(candidate)) {
            idx_t r = candidate % 24;
            if (r == 1 || r == 19) {
                ++count;
                if (count == n) return (seq_t)candidate;
            }
        }
        candidate += 2; /* skip evens */
    }
}
