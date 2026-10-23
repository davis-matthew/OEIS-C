/*
 * A105126  Primes of the form 16n+9.
 * https://oeis.org/A105126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 800, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 16n+9 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* offset 1: a(1) is the first term 41 */
    if (n == 0) return 0;             /* undefined for n==0, but return 0 */
    if (n == 1) return (seq_t)41;     /* first prime 16*2+9 */

    uint64_t count = 1;  /* we already have the first prime */
    uint64_t k = 2;       /* current k such that 16*k+9 is inspected */

    while (1) {
        k++;                                 /* next k */
        uint64_t p = 16ULL * k + 9ULL;       /* candidate prime */
        if (is_prime(p)) {
            count++;                         /* found a prime of the form 16n+9 */
            if (count == n) return (seq_t)p; /* desired term */
        }
    }
}
