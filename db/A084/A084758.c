/*
 * A084758  The slowest increasing sequence of primes such that difference of
 *          successive terms is unique.
 * https://oeis.org/A084758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 257, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A084758:
   a(1)=2; for n>1, a(n) is the smallest prime greater than a(n-1)
   such that the difference a(n)-a(n-1) has not appeared among
   previous differences. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/isPrime.h"          /* provides bool isPrime(idx_t) */

#define MAX_TERMS 20000                  /* > required 10000 */

/* static storage for the sequence and used differences         */
static seq_t  a_terms[MAX_TERMS];
static seq_t  diff_terms[MAX_TERMS-1];
static size_t terms_len = 0;

/* helper: linear search for a diff value                       */
static bool diff_used(seq_t diff, size_t start, size_t count)
{
    for (size_t i = start; i < count; ++i)
        if (diff_terms[i] == diff)
            return true;
    return false;
}

extern seq_t a(idx_t n)
{
    if (n == 0)                    /* offset is 1; caller should use n>=1 */
        return (seq_t)0;

    /* initialise sequence with first term */
    if (terms_len == 0) {
        a_terms[0] = (seq_t)2;
        terms_len = 1;
    }

    /* generate terms up to requested n */
    while (terms_len < n && terms_len < MAX_TERMS) {
        idx_t prev = (idx_t)a_terms[terms_len - 1];
        idx_t cand = (prev == (idx_t)2) ? (idx_t)3 : prev + (idx_t)2;

        /* search for the next term */
        while (true) {
            if (isPrime(cand)) {
                seq_t diff = (seq_t)(cand - prev);
                if (!diff_used(diff, 0, terms_len - 1)) {
                    a_terms[terms_len] = (seq_t)cand;
                    diff_terms[terms_len - 1] = diff;
                    ++terms_len;
                    break;
                }
            }
            cand += (idx_t)2;          /* next odd candidate */
        }
    }

    /* if requested n exceeds capacity, return the last computed term */
    if (n > terms_len)
        return a_terms[terms_len - 1];

    return a_terms[n - 1];
}
