/*
 * A076354  Numbers n such that 210*n-1 is prime.
 * https://oeis.org/A076354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A076354: numbers n such that 210*n - 1 is prime */

#include <stdlib.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *terms = NULL;
    static size_t len   = 0;            /* number of terms stored */
    static size_t cap   = 0;            /* allocated capacity */
    static idx_t next = 2;              /* next candidate n to test */

    /* Grow cache if needed for current request */
    while (len < n) {
        /* check 210*n - 1 for primality */
        unsigned long long val = 210ULL * (unsigned long long)next - 1ULL;
        if (is_prime_u64(val)) {
            /* store the qualifying n */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failed */
                terms = tmp;
                cap = new_cap;
            }
            terms[len++] = (seq_t)next;
        }
        ++next;
    }
    return terms[n - 1];
}
