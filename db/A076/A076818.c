/*
 * A076818  Lexicographically earliest sequence of pairwise coprime
 *          tetrahedral numbers.
 * https://oeis.org/A076818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 33, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076818: lexicographically earliest sequence of pairwise coprime
 * tetrahedral numbers. Computed by incremental search. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"       /* Provides seq_t gcd(seq_t, seq_t) */

static seq_t *terms = NULL;     /* stored sequence values */
static size_t terms_len = 0;    /* how many have been found */
static size_t terms_cap = 0;    /* current capacity of the array */
static idx_t next_k = 1;        /* next tetrahedral index to try */

/* Ensure the dynamic array has at least 'needed' slots. */
static void ensure_capacity(size_t needed)
{
    if (terms_cap < needed) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 8;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms = tmp;
        terms_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                       /* offset is 1 – safety */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        seq_t a1 = next_k;             /* k            */
        seq_t b1 = next_k + 1;         /* k + 1        */
        seq_t c1 = next_k + 2;         /* k + 2        */

        /* Divide the product by 2 and 3 as early as possible to avoid
         * intermediate overflow. */
        if ((a1 & (seq_t)1) == 0)
            a1 /= 2;
        else if ((b1 & (seq_t)1) == 0)
            b1 /= 2;
        else
            c1 /= 2;

        if (a1 % 3 == 0)
            a1 /= 3;
        else if (b1 % 3 == 0)
            b1 /= 3;
        else
            c1 /= 3;

        seq_t candidate = a1 * b1 * c1;     /* tetrahedral number */

        /* Check coprimeness with all previously chosen terms. */
        bool coprime = true;
        for (size_t i = 0; i < terms_len; ++i) {
            if (gcd(candidate, terms[i]) != (seq_t)1) {
                coprime = false;
                break;
            }
        }

        if (coprime) {
            ensure_capacity(terms_len + 1);
            terms[terms_len++] = candidate;
        }
        ++next_k;    /* move to the next k */
    }

    return terms[n - 1];   /* zero‑based array stores a(1) at index 0 */
}
