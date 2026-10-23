/*
 * A045354  Primes congruent to {0, 2, 5, 6} mod 7.
 * https://oeis.org/A045354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based computation: primes ≡ {0, 2, 5, 6} (mod 7). */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence defined for n ≥ 1 */

    /* static storage for already computed qualifying primes */
    static seq_t *qual = NULL;
    static size_t qual_cap = 0;
    static size_t qual_sz  = 0;
    static idx_t  last_checked = 2;

    if (qual_cap == 0) {
        qual_cap = 16;
        qual = (seq_t *)malloc(qual_cap * sizeof(seq_t));
        if (!qual) return 0; /* allocation failed */
    }

    /* generate qualifying primes until we have at least n of them */
    while (qual_sz < n) {
        if (is_prime(last_checked)) {
            idx_t r = last_checked % 7;
            if (r == 0 || r == 2 || r == 5 || r == 6) {
                if (qual_sz == qual_cap) {
                    qual_cap <<= 1; /* double capacity */
                    seq_t *tmp = (seq_t *)realloc(qual, qual_cap * sizeof(seq_t));
                    if (!tmp) return 0; /* allocation failed */
                    qual = tmp;
                }
                qual[qual_sz++] = (seq_t)last_checked;
            }
        }
        /* advance to next candidate (skip even numbers after 2) */
        if (last_checked == 2)
            last_checked = 3;
        else
            last_checked += 2;
    }

    return qual[n - 1];
}
