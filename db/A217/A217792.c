/*
 * A217792  Primes which are equal to the sum of the binary digits of
 *          consecutive primes starting with 2.
 * https://oeis.org/A217792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 802, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS sequence A217792.
 *  a(n) is the nth prime that equals the sum of the binary digits
 *  of the first k consecutive primes starting with 2, for the
 *  smallest k such that the cumulative sum is prime.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"              /* bool is_prime(idx_t x); */
#include "../utils/alloc_seq_array.h"       /* seq_t *alloc_seq_array(size_t n); */
#include "../utils/ensure_capacity.h"       /* void ensure_capacity(size_t needed,
                                               size_t *cap, void **ptr,
                                               size_t elem_size); */

static seq_t *terms = NULL;          /* memoized sequence terms   */
static size_t  terms_cnt = 0;        /* number of terms stored    */
static size_t  terms_cap = 0;        /* allocated capacity        */

static uint64_t last_prime = 1;      /* last prime examined       */
static seq_t   cur_sum   = 0;        /* running sum of popcounts  */

/* Return a(n).  n is 1‑based; a(0) is undefined and returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Lazy initialization of storage and state. */
    if (terms == NULL) {
        terms_cap = 16;
        terms = alloc_seq_array(terms_cap);
        terms_cnt = 0;
        last_prime = 1;
        cur_sum = 0;
    }

    /* Generate additional terms until the desired index is reached. */
    while (terms_cnt < (size_t)n) {
        /* Find the next prime after last_prime. */
        uint64_t p = last_prime + 1;
        if (p < 2) p = 2;
        while (!is_prime((idx_t)p))
            ++p;
        last_prime = p;

        /* Add the binary digit sum (popcount) of this prime. */
        seq_t pop = (seq_t)__builtin_popcountll(p);
        cur_sum += pop;

        /* If the current sum itself is prime, record it. */
        if (is_prime((idx_t)cur_sum)) {
            /* Ensure there is room for a new element. */
            ensure_capacity(terms_cnt + 1, &terms_cap,
                            (void **)&terms, sizeof(seq_t));
            terms[terms_cnt++] = cur_sum;
        }
    }

    return terms[n - 1];
}
