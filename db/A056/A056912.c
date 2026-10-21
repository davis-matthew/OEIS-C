/*
 * A056912  Odd squarefree numbers for which the number of prime divisors is
 *          odd.
 * https://oeis.org/A056912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   odd squarefree numbers that have an odd number of distinct prime
 *            divisors
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

static idx_t *spf = NULL;          /* smallest prime factor array */
static idx_t spf_limit = 0;        /* current maximum n covered by spf */

/* ensure the spf array covers all numbers up to <limit> */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;
    /* rebuild from scratch – the maximal limit reached is small (≈ 10⁶ for the
       required problem size) so this is inexpensive. */
    if (spf) {
        free(spf);
        spf = NULL;
    }
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* return the n-th term of A056912 */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    idx_t count   = 0;      /* how many terms we've found */
    idx_t cand    = 3;      /* current odd candidate */

    /* We lazily extend the SPF table when we hit its limit. */
    while (1) {
        if (cand > spf_limit)
            ensure_spf(cand * 2 > spf_limit ? cand * 2 : spf_limit + 100);

        /* factorise cand using the SPF table */
        idx_t m = cand;
        int   distinct = 0;
        bool  squarefree = true;

        while (m > 1 && squarefree) {
            idx_t p = spf[m];
            distinct++;
            m /= p;
            if (m % p == 0) {          /* repeated prime factor -> composite square */
                squarefree = false;
                break;
            }
        }

        if (squarefree && (distinct & 1)) {   /* odd number of prime factors */
            ++count;
            if (count == n)
                return (seq_t)cand;
        }

        cand += 2;     /* only odd numbers can be squarefree with odd number of primes */
    }
}
