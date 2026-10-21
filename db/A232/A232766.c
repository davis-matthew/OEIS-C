/*
 * A232766  Numbers n such that the concatenation n987654321 is prime.
 * https://oeis.org/A232766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) consists of all integers n such that the concatenation
 *            n987654321 is prime. a(1) = 28, a(2) = 32, ... This function
 *            computes the n‑th such integer
 * Range:     u8 none, u16 none, u32 none, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static cache of already discovered terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* current allocated array size   */
    static size_t count   = 0;    /* number of terms stored         */
    static idx_t next_candidate = 1; /* smallest n not yet examined */

    /* generate terms until we have at least n of them */
    while (count < n) {
        /* concatenate candidate with 987654321 : candidate*10^9 + 987654321 */
        seq_t concat = (seq_t)next_candidate * 1000000000ULL + 987654321ULL;

        /* test primality of the concatenated number */
        if (isPrimeGeneric(concat)) {
            /* grow cache if necessary */
            if (count == capacity) {
                size_t newcap = capacity ? capacity * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) {
                    /* out of memory – terminate the program */
                    abort();
                }
                cache = tmp;
                capacity = newcap;
            }
            cache[count++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];   /* cache[0] is a(1) */
}
