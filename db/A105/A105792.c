/*
 * A105792  Largest prime <= numbers congruent (2,4) mod 6 (duplicates
 *          removed).
 * https://oeis.org/A105792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = largest prime ≤ m where m is the n‑th integer with m≡2 or
 *            4 (mod 6), duplicates removed
 * Range:     u8 n <= 38, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *terms = NULL;
    static idx_t terms_size = 0;
    static idx_t terms_cap = 0;
    static uint64_t max_n = 1;            /* last integer examined          */
    static seq_t current_prime = 0;       /* largest prime ≤ max_n          */
    static seq_t last_output = 0;         /* last value appended to the list */

    while (terms_size < n)
    {
        ++max_n;
        if (is_prime(max_n))
            current_prime = (seq_t)max_n;

        if (max_n % 6 == 2 || max_n % 6 == 4)
        {
            if (current_prime != last_output)
            {
                if (terms_size == terms_cap)
                {
                    size_t new_cap = terms_cap ? terms_cap * 2 : 128;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) abort();  /* allocation failure */
                    terms = tmp;
                    terms_cap = new_cap;
                }
                terms[terms_size++] = current_prime;
                last_output = current_prime;
            }
        }
    }

    return terms[n - 1];
}
