/*
 * A181173  Primes whose base 5 representation does not contain a 0.
 * https://oeis.org/A181173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose base‑5 representation contains no 0 digit. */
/* Computed by a simple incremental search using is_prime_big(). */

#include <stdbool.h>
#include <stdlib.h>          /* for size_t */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Static cache: enough for n up to at least 10000 */
    static seq_t cache[200000];
    static size_t cache_size = 0;
    static seq_t candidate = 2;          /* next integer to test */

    /* Ensure we have at least n terms */
    while (cache_size < n) {
        seq_t cand = candidate;
        bool prime = false;

        if (cand == 2)
            prime = true;                 /* 2 is prime */
        else if (cand % 2 != 0)
            prime = is_prime_big(cand);   /* test odd candidates only */

        if (prime) {
            seq_t t = cand;
            bool no_zero = true;
            while (t > 0) {
                if (t % 5 == 0) { no_zero = false; break; }
                t /= 5;
            }
            if (no_zero)
                cache[cache_size++] = cand;
        }

        /* Advance to next candidate */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;   /* skip even numbers after 2 */
    }

    return cache[n - 1];
}
