/*
 * A062800  Primes of form 100*k + 1.
 * https://oeis.org/A062800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth prime that is ≡ 1 (mod 100) (i.e. the nth prime
 *            of the form 100·k + 1)
 * Range:     u8 n <= 1, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cache of already discovered terms   */
    static seq_t *terms = NULL;     /* array of primes found so far */
    static idx_t terms_cap = 0;     /* allocated capacity of 'terms' */
    static idx_t terms_size = 0;    /* number of primes currently stored */

    /* The next value of k to test (k starts at 1)   */
    static idx_t next_k = 1;

    /* n is 1‑based; ignore n==0 (undefined for this sequence) */
    if (n == 0) return 0;

    /* Grow the cache if necessary */
    if (terms_cap < terms_size + 1) {
        idx_t new_cap = terms_cap ? terms_cap * 2 : 4;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }

    /* Find more terms until we reach the requested index */
    while (terms_size < n) {
        /* candidate prime of the form 100·k + 1 */
        seq_t cand = (seq_t)100 * (seq_t)next_k + 1;

        bool prime;
        if (sizeof(seq_t) <= 8) {
            /* 64‑bit candidate   */
            prime = is_prime_u64((uint64_t)cand);
        } else {
            /* arbitrary‑precision candidate   */
            prime = is_prime_big(cand);
        }

        if (prime) {
            terms[terms_size++] = cand;
            /* cache grew earlier if needed */
            if (terms_size >= terms_cap) {
                idx_t new_cap = terms_cap * 2;
                terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                terms_cap = new_cap;
            }
        }
        ++next_k;   /* advance to the next k regardless of primality */
    }

    return terms[n - 1];
}
