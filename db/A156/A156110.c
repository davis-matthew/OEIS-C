/*
 * A156110  Primes p such that p+18 and p+72 are both prime.
 * https://oeis.org/A156110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 400, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Provides primes p such that p+18 and p+72 are also prime.
   The function computes terms on demand, memoizing the results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_generic.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; behaviour below offset is irrelevant */

    /* static cache for the sequence values */
    static seq_t *table   = NULL;
    static size_t table_len = 0;
    static size_t table_cap = 0;

    /* next odd candidate to test for primality (2 is handled implicitly) */
    static uint64_t cand = 3;

    while (table_len < (size_t)n) {
        /* find the next prime p */
        uint64_t p;
        for (;;) {
            if (is_prime_generic(cand)) {
                p = cand;
                cand += 2;          /* next odd number */
                break;
            }
            cand += 2;
        }

        /* check the two required offsets */
        if (is_prime_generic(p + 18) && is_prime_generic(p + 72)) {
            /* grow table if needed */
            if (table_len == table_cap) {
                size_t new_cap = (table_cap == 0) ? 64 : table_cap * 2;
                seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
                if (!tmp) return 0;  /* memory allocation failed */
                table = tmp;
                table_cap = new_cap;
            }
            table[table_len++] = (seq_t)p;
        }
    }

    return table[n - 1];
}
