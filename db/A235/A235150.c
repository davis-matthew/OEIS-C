/*
 * A235150  Primes p such that (p reversed) + 9 is also a prime.
 * https://oeis.org/A235150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 278, u32 n <= 278, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235150: primes p such that rev(p)+9 is also prime
   Implementation: sequential search with memoisation. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* sequence offset is 1 – undefined for n=0 */
        return 0;

    static seq_t *vals = NULL;      /* storage of found primes */
    static size_t vals_cap = 0;     /* capacity of vals array   */
    static size_t vals_size = 0;    /* number of terms stored   */
    static seq_t last_candidate = 1;/* last number examined   */

    /* extend the list until we have at least n terms */
    while (vals_size < n) {
        seq_t cand = last_candidate + 1ULL;  /* next number to test */

        /* primality test for the candidate */
        if (is_prime_big(cand)) {
            seq_t revp   = reverse_decimal(cand);
            seq_t target = revp + (seq_t)9;

            if (is_prime_big(target)) {
                /* grow the array if needed */
                if (vals_size == vals_cap) {
                    size_t new_cap = vals_cap ? vals_cap << 1 : 4;
                    seq_t *tmp = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
                    if (!tmp)                 /* allocation failed */
                        return 0;
                    vals = tmp;
                    vals_cap = new_cap;
                }
                vals[vals_size++] = cand;       /* store the term */
            }
        }

        last_candidate = cand;  /* continue from this number next time */
    }

    return vals[n - 1];   /* indexes start at 1 in OEIS */
}
