/*
 * A156345  Nonprimes with the same number of even digits and prime digits.
 * https://oeis.org/A156345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = the nth nonprime whose number of even digits (2,4,6,8)
   equals the number of prime digits (2,3,5,7).  
   The function builds the sequence on demand and keeps the results
   in a static cache so that repeated calls are fast.
*/
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: count even and prime digits of a number */
static void count_even_prime_digits(seq_t x, unsigned int *even_cnt, unsigned int *prime_cnt)
{
    *even_cnt  = 0;
    *prime_cnt = 0;

    if (x == 0) {
        /* digit 0 is neither even nor prime */
        return;
    }

    while (x > 0) {
        unsigned int d = (unsigned int)(x % 10);
        if (d == 2 || d == 4 || d == 6 || d == 8) (*even_cnt)++;
        if (d == 2 || d == 3 || d == 5 || d == 7) (*prime_cnt)++;
        x /= 10;
    }
}

/* Helper: test primality of a value that fits in seq_t (typically up to 64‑bit). */
static bool is_prime_local(seq_t n)
{
    if ((seq_t)n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;   /* even number greater than 2 is composite */

    /* simple trial division up to sqrt(n) */
    for (seq_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;
    static idx_t  next_candidate = 0;   /* next integer to examine */

    /* Grow cache if needed */
    while (size < (size_t)n) {
        seq_t cand = (seq_t)next_candidate;

        /* test nonprime */
        if (!is_prime_local(cand)) {
            unsigned int even_cnt, prime_cnt;
            count_even_prime_digits(cand, &even_cnt, &prime_cnt);

            if (even_cnt == prime_cnt) {
                /* enlarge cache when full */
                if (size == cap) {
                    size_t new_cap = cap ? cap * 2 : 4;
                    seq_t *new_buf = mem_new(seq_t, new_cap);
                    if (!new_buf) exit(1);
                    if (cache) {
                        for (size_t i = 0; i < size; ++i) new_buf[i] = cache[i];
                        mem_free(cache);
                    }
                    cache = new_buf;
                    cap   = new_cap;
                }
                cache[size++] = cand;
            }
        }
        next_candidate++;
    }
    return cache[n - 1];
}
