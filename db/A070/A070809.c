/*
 * A070809  Cototient(totient(n))=A070556(n) is not a power of 2 and n is not
 *          a prime number.
 * https://oeis.org/A070809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m ≥ 1 that is not prime and whose
 *            cototient of φ(m) (i.e. φ(m)−φ(φ(m))) is not a power of 2
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_power_of_two, is_prime, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/phi.h"
#include "../utils/is_prime.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    /* Static cache holding the found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 0;

    if (n == 0)  /* 0 is outside the sequence's offset; return 0 safely */
        return (seq_t)0;

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        ++last_checked;            /* candidate integer m */

        /* Skip primes */
        if (is_prime(last_checked))
            continue;

        /* Compute φ(m) */
        seq_t phi_m  = phi((idx_t)last_checked);

        /* Compute φ(φ(m)) */
        seq_t phi_phi_m = phi((idx_t)phi_m);

        /* Cototient of φ(m) */
        seq_t cototient = phi_m - phi_phi_m;

        /* Accept if cototient is not a power of two */
        if (!is_power_of_two(cototient)) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!tmp) { abort(); }  /* allocation failure – halt */
                if (cache) {
                    memcpy(tmp, cache, cache_len * sizeof(seq_t));
                    free(cache);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_checked;
        }
    }

    /* cache is 0‑based: a(1) is cache[0] */
    return cache[n - 1];
}
