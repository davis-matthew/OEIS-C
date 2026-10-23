/*
 * A133412  Sum of primes between n-th and (n+1)-th semiprimes.
 * https://oeis.org/A133412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   build_spf, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A133412:
   a(n) = sum of all primes p with s(n) < p < s(n+1),
   where s(k) is the k‑th semiprime (product of two primes,
   possibly equal).  For example a(1)=5 because 5 is the
   only prime between the 1st semiprime 4 and the 2nd semiprime 6. */

#include "../utils/build_spf.h"
#include "../utils/is_prime.h"
#include <stdlib.h>
#include <stdbool.h>

/* Cached data for semiprimes --------------------------------------- */

static idx_t *spf = NULL;          /* array of smallest prime factors */
static idx_t  spf_limit = 0;       /* limit used for current SPF array */

static idx_t *semiprimes = NULL;   /* semiprime list, 1‑based (semiprimes[1] = s(1)) */
static size_t semip_cnt = 0;       /* number of semiprimes stored */
static size_t semip_cap = 0;       /* capacity of semiprimes array */

/* Grow the semiprime array if needed. */
static void grow_semiprimes(size_t needed)
{
    if (needed <= semip_cap) return;
    size_t new_cap = semip_cap ? semip_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    idx_t *tmp = (idx_t *)realloc(semiprimes, new_cap * sizeof(idx_t));
    if (!tmp) exit(EXIT_FAILURE);
    semiprimes = tmp;
    semip_cap  = new_cap;
}

/* Ensure that at least (n+1) semiprimes are available (we need s(n) and s(n+1)). */
static void ensure_semiprimes(idx_t n)
{
    if (semip_cnt >= (size_t)(n + 1)) return;           /* already sufficient */

    idx_t limit = (n < 50) ? 200 : n * 20 + 100;        /* rough initial bound */

    while (1) {
        /* Rebuild SPF array for the new limit. */
        if (spf) { free(spf); spf = NULL; }
        build_spf(limit, &spf);
        if (!spf) exit(EXIT_FAILURE); /* allocation failed */

        /* Allocate or resize the semiprimes buffer. */
        grow_semiprimes(limit + 2);    /* +2 so that index n+1 is reachable */

        /* Generate semiprimes up to 'limit'. */
        size_t count = 0;              /* semiprimes will be stored starting at 1 */
        for (idx_t x = 2; x <= limit; ++x) {
            idx_t p = spf[x];
            if (!p) continue;          /* x is 0 or 1 */
            idx_t q = x / p;
            if (is_prime(q)) {
                ++count;
                semiprimes[count] = x;  /* store at 1‑based index */
            }
        }

        semip_cnt = count;

        if (semip_cnt >= (size_t)(n + 1)) return;    /* we have enough */

        /* Not enough semiprimes; double the limit and try again. */
        limit *= 2;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                      /* undefined for n < 1 */

    ensure_semiprimes(n);

    idx_t low  = semiprimes[n];        /* s(n)   */
    idx_t high = semiprimes[n + 1];    /* s(n+1) */

    seq_t sum = 0;

    for (idx_t p = low + 1; p < high; ++p) {
        if (is_prime(p)) sum += (seq_t)p;
    }
    return sum;
}
