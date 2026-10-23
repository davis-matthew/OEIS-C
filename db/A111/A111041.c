/*
 * A111041  Numbers m such that 2*m^2 + 25 is prime.
 * https://oeis.org/A111041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A111041: Numbers m such that 2*m^2 + 25 is prime.
   The sequence is defined by a predicate, not by a closed‑form formula.
   We generate terms on demand, caching them for subsequent calls. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* caching the found terms */
    static seq_t *terms = NULL;
    static size_t terms_size = 0, terms_cap = 0;
    static idx_t last_checked = 0;  /* last m that was tested */

    if (n == 0)
        return 0;  /* undefined, but avoid undefined behaviour */

    /* Ensure we have at least n terms */
    while (terms_size < (size_t)n) {
        /* find next candidate m that is divisible by 3 */
        do {
            last_checked++;
        } while (last_checked % 3 != 0);

        /* compute 2*m^2 + 25 (fits in uint64_t for our expected range) */
        uint64_t m = (uint64_t)last_checked;
        uint64_t val = 2ULL * m * m + 25ULL;

        /* test primality */
        if (is_prime(val)) {
            /* store m in the terms array */
            if (terms_size == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_size++] = (seq_t)m;
        }
    }

    return terms[n - 1];
}
