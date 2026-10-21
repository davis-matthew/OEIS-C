/*
 * A072281  Numbers n such that phi(n) + 1 and phi(n) - 1 are twin primes.
 * https://oeis.org/A072281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 3896, u32 n <= 5538, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A072281: numbers n such that phi(n)-1 and phi(n)+1 are twin primes.
 * The function generates values on demand, caching results for future calls.
 * It keeps a search that increments n until enough terms are found.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/calc_phi.h"
#include "../utils/is_prime_big.h"

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static idx_t last_checked = 0;   /* highest n examined so far */

    while (len < n) {
        /* Examine the next candidate n */
        last_checked++;
        /* Compute φ(n) */
        seq_t phi_val = calc_phi((seq_t)last_checked);

        /* φ(n) < 3 → φ(n)-1 < 2, so not a twin‑prime pair */
        if (phi_val < 3)
            continue;

        seq_t low  = phi_val - 1;
        seq_t high = phi_val + 1;

        /* Both φ(n)-1 and φ(n)+1 must be prime */
        if (is_prime_big(low) && is_prime_big(high)) {
            /* Append to the cache */
            if (len == cap) {
                cap = cap == 0 ? 8 : cap * 2;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[len++] = (seq_t)last_checked;
        }
    }

    return terms[n - 1];
}
