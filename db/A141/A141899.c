/*
 * A141899  Primes of the form 2*3*5*7*k + 97.
 * https://oeis.org/A141899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime of the form 210*k + 97 (k ≥ 0)
 * Range:     u8 n <= 1, u16 n <= 142, u32 n <= 51, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of primes found so far */
    static seq_t *cache = NULL;      /* array of primes   */
    static size_t cached = 0;        /* number stored    */
    static size_t cap = 0;           /* current capacity */
    static idx_t next_k = 0;         /* next value of k  */

    if (n == 0)  /* undefined; return 0 for safety */
        return 0;

    /* zero‑based index of the wanted term */
    size_t target = (size_t)(n) - 1;

    /* expand cache until we have enough primes */
    while (cached <= target) {
        /* candidate prime = 210*k + 97 */
        seq_t candidate = (seq_t)97 + ((seq_t)210 * (seq_t)next_k);

        /* test primality (works for all seq_t sizes) */
        if (is_prime_big(candidate)) {
            if (cached >= cap) {
                /* grow capacity */
                size_t new_cap = cap == 0 ? 16 : cap * 2;
                if (new_cap <= cached)
                    new_cap = cached + 1;
                seq_t *fresh = seq_realloc(cache, cap, new_cap);
                cache = fresh;
                cap = new_cap;
            }
            cache[cached++] = candidate;
        }
        next_k++;          /* advance to the next k */
    }

    return cache[target];
}
