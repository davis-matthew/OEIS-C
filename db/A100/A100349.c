/*
 * A100349  Numbers n such that n-2^k is a prime or semiprime for all k > 0
 *          with 2^k < n.
 * https://oeis.org/A100349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that n-2^k is a prime or semiprime for all k>0 with 2^k < n
   The function builds the sequence incrementally and caches results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

/* Return true iff x is a semiprime (exactly two prime factors, not necessarily distinct). */
static bool is_semiprime(seq_t x)
{
    if (x < 4) return false;               /* 4 = 2*2 is the smallest semiprime */
    if (is_prime_seq(x)) return false;      /* primes are not semiprime */

    seq_t d = 2;
    while (d * d <= x) {
        if (x % d == 0) {
            seq_t q = x / d;
            return is_prime_seq(d) && is_prime_seq(q);
        }
        d = d + 1;
    }
    return false;   /* composite with more than two prime factors */
}

seq_t a(idx_t n)
{
    /* Static cache of already found terms */
    static seq_t *terms   = NULL;
    static idx_t term_cnt = 0;
    static idx_t alloc_sz = 0;
    static idx_t next_candidate = 3;   /* first candidate to test will become 4 */

    /* Allocate storage on first call */
    if (terms == NULL) {
        alloc_sz = 128;
        terms = (seq_t *)malloc(alloc_sz * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (term_cnt < n) {
        next_candidate = next_candidate + 1; /* candidate starts from 4 */
        seq_t cand = (seq_t)next_candidate;
        bool ok = true;

        /* Check condition for all k with 2^k < cand */
        for (idx_t pow = 2; pow < next_candidate; pow <<= 1) {
            seq_t diff = cand - (seq_t)pow;
            if (!is_prime_seq(diff) && !is_semiprime(diff)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            if (term_cnt >= alloc_sz) {
                alloc_sz *= 2;
                terms = (seq_t *)realloc(terms, alloc_sz * sizeof(seq_t));
            }
            terms[term_cnt++] = cand;
        }
    }

    return terms[n - 1];   /* 0‑based index */
}
