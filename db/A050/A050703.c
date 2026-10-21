/*
 * A050703  Numbers that when added to the sum of their prime factors (with
 *          multiplicity) become prime.
 * https://oeis.org/A050703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 7046, u32 n <= 9670, u64 n <= 9670,
 *            u128 n <= 9670, big any n
 * Helpers:   append_term, is_prime, sum_prime_factors_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function finds numbers n such that n plus the sum of their prime
    factors (with multiplicity) is prime.  It stores each found value
    in a static array and returns the nth term (n ≥ 1) on demand.  */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/append_term.h"
#include "../utils/sum_prime_factors_uint64.h"

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Static state: the list of already found terms. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t candidate = 6;   /* first integer that can satisfy the criterion */

    if (n == 0)  /* the sequence is defined for n ≥ 1; return 0 for n < 1 */
        return (seq_t)0;

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        uint64_t sum_pf = sum_prime_factors_uint64(candidate);
        uint64_t tot    = candidate + sum_pf;

        if (is_prime((idx_t)tot))
            append_term(&terms, &terms_len, &terms_cap, (seq_t)candidate);

        candidate++;
    }

    return terms[n - 1];
}
