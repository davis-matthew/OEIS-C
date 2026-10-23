/*
 * A106699  Numbers k such that 1009*k - 9973 is prime.
 * https://oeis.org/A106699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that 1009*k - 9973 is prime */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    /* Order independent memoising */
    static seq_t *terms = NULL;
    static idx_t term_count = 0;
    static idx_t term_cap = 0;
    static idx_t last_k = 0;

    /* Grow capacity as needed */
    if (term_cap == 0) {
        term_cap = 16;
        terms = (seq_t *)malloc(sizeof(seq_t) * term_cap);
    }

    /* Generate terms until we have the nth one */
    while (term_count < n) {
        ++last_k;
        /* p = 1009*last_k - 9973 */
        unsigned __int128 prod = (unsigned __int128)1009 * (unsigned __int128)last_k;
        seq_t p = (seq_t)(prod - 9973);

        /* Check for primality (p fits in uint64_t for the required range) */
        if (is_prime_uint64((uint64_t)p)) {
            if (term_count == term_cap) {
                term_cap <<= 1;
                terms = (seq_t *)realloc(terms, sizeof(seq_t) * term_cap);
            }
            terms[term_count++] = (seq_t)last_k;
        }
    }

    return terms[n - 1];
}
