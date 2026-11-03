/*
 * A102373  Numbers k such that k33333 is prime.
 * https://oeis.org/A102373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates A102373: numbers k such that the decimal
   concatenation k33333 is prime.
   The algorithm simply tests successive integers k, forms the number
   k*100000 + 33333, and uses a prime test from the utilities. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) {            /* undefined for n < 1 */
        return 0;
    }

    /* static state for memoisation */
    static seq_t *cache      = NULL;
    static idx_t capacity    = 0;
    static idx_t count       = 0;
    static seq_t last_k      = 0;   /* last candidate examined */

    /* initialise cache on first call */
    if (cache == NULL) {
        capacity = 16;
        cache = mem_new(seq_t, capacity);
        if (cache == NULL) {
            return 0;          /* allocation failure */
        }
    }

    /* find terms until we have the requested one */
    while (count < n) {
        /* next candidate k */
        ++last_k;

        /* form k33333 */
        seq_t candidate = last_k * (seq_t)100000 + 33333;

        /* test for primality */
        if (is_prime_seq(candidate)) {
            /* enlarge cache if needed */
            if (count >= capacity) {
                idx_t new_cap = capacity * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (tmp == NULL) {
                    mem_free(cache);
                    return 0;  /* allocation failure */
                }
                cache = tmp;
                capacity = new_cap;
            }
            cache[count++] = last_k;
        }
    }

    return cache[(size_t)(n - 1)];
}
