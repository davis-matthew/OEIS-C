/*
 * A157695  Composite numbers that are not multiples of 3.
 * https://oeis.org/A157695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   composite numbers that are not multiples of 3 a(n) = the n‑th
 *            integer that is composite and not divisible by 3 (sequence is
 *            the intersection of all composites with the set of non‑multiples
 *            of 3)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static idx_t sieve_limit = 0;          /* largest number that has been sieved */
static bool *prime_flags = NULL;       /* sieve flags for 0..sieve_limit */
static seq_t *comps = NULL;            /* list of found composites not divisible by 3 */
static size_t comps_cnt = 0;           /* number of composites stored so far */
static size_t comps_cap = 0;           /* allocated capacity of comps[] */

/* Ensure sieve up to at least 'limit'.
   Rebuilds the entire sieve and the composite list each time, keeping
   the computation simple and correct for any seq_t type. */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    free(prime_flags);
    prime_flags = (bool*)malloc((size_t)(limit + 1));
    if (!prime_flags)
        return; /* allocation failure – should never happen for the intended use */

    for (size_t i = 0; i <= (size_t)limit; ++i)
        prime_flags[i] = true;

    if (limit >= 0) prime_flags[0] = false;
    if (limit >= 1) prime_flags[1] = false;

    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (prime_flags[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_flags[j] = false;
        }
    }

    /* rebuild composite list */
    comps_cnt = 0;
    for (idx_t v = 4; v <= limit; ++v) {
        if (!prime_flags[v] && v % 3 != 0) {
            if (comps_cnt == comps_cap) {
                size_t new_cap = comps_cap ? comps_cap * 2 : 1024;
                seq_t *tmp = (seq_t*)realloc(comps, new_cap * sizeof(seq_t));
                if (!tmp) return; /* allocation failure – should not happen */
                comps = tmp;
                comps_cap = new_cap;
            }
            comps[comps_cnt++] = (seq_t)v;
        }
    }
    sieve_limit = limit;
}

/* Return the n-th term of A157695.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 0, but harmless */

    /* make sure we have at least n composites available */
    while (comps_cnt < n) {
        idx_t new_limit = sieve_limit ? sieve_limit * 2 : 100;
        if (new_limit < n * 4) new_limit = n * 4 + 100;
        ensure_sieve(new_limit);
    }
    return comps[n - 1];
}
