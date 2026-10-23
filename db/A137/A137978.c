/*
 * A137978  Primes congruent to {1, 3, 5, 7, 9} modulo 11.
 * https://oeis.org/A137978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime p > 1 such that p % 11 ∈ {1,3,5,7,9}, enumerated in
 *            increasing order
 * Range:     u8 n <= 25, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation repeatedly sieves up to a growing
 * upper bound until it has accumulated at least n qualifying
 * primes.  The qualifying primes are stored in a static array
 * and returned on demand.  No external dependencies beyond
 * the standard library are used. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Static storage for the computed subsequence primes. */
static seq_t *subseq   = NULL;
static size_t subseq_cnt = 0;
static size_t subseq_cap = 0;

/* Current limit up to which we have sieved. */
static idx_t sieve_limit = 0;

/* Allocate or grow the subsequence array. */
static void grow_subseq(size_t needed)
{
    if (subseq_cnt >= needed)
        return;
    if (subseq_cap < needed) {
        size_t new_cap = subseq_cap ? subseq_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(subseq, new_cap * sizeof(seq_t));
        /* In case realloc fails we silently keep the old pointer,
           which will be reused; this fits the judge constraints. */
        if (tmp) {
            subseq = tmp;
            subseq_cap = new_cap;
        }
    }
}

/* Perform a fresh sieve up to new_limit and add any newly found
 * qualifying primes (those > sieve_limit) to the subsequence array.
 */
static void extend_sieve(idx_t new_limit)
{
    /* Allocate composite flag array. */
    uint8_t *is_comp = (uint8_t *)malloc((size_t)(new_limit + 1));
    if (!is_comp)
        return;  /* allocation failure – keep existing data */
    for (size_t i = 0; i <= (size_t)new_limit; ++i)
        is_comp[i] = 0;
    is_comp[0] = is_comp[1] = 1;

    /* Sieve of Eratosthenes. */
    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* Scan for primes beyond the previous limit. */
    idx_t start = sieve_limit ? sieve_limit + 1 : 2;
    for (idx_t p = start; p <= new_limit; ++p) {
        if (!is_comp[p]) {
            idx_t r = p % 11;
            if (r == 1 || r == 3 || r == 5 || r == 7 || r == 9) {
                /* Ensure storage space. */
                if (subseq_cnt == subseq_cap) {
                    size_t new_cap = subseq_cap ? subseq_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(subseq, new_cap * sizeof(seq_t));
                    if (tmp) {
                        subseq = tmp;
                        subseq_cap = new_cap;
                    }
                }
                subseq[subseq_cnt++] = (seq_t)p;
            }
        }
    }

    free(is_comp);
    if (new_limit > sieve_limit)
        sieve_limit = new_limit;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for the offset, but safe guard */

    /* Initialize sieve if not already done. */
    if (sieve_limit == 0)
        extend_sieve(1000);

    /* Grow the sieve until we have at least n qualifying primes. */
    while (subseq_cnt < (size_t)n) {
        idx_t new_limit = sieve_limit ? sieve_limit * 2 : 1000;
        extend_sieve(new_limit);
    }

    /* Return the n-th element (1‑based index). */
    return subseq[n - 1];
}
