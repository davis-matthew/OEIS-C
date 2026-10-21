/*
 * A070258  Smallest of 3 consecutive numbers each divisible by a square.
 * https://oeis.org/A070258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A235578(n) – 1, where a(n) is the nth smallest integer m
 *            such that m, m+1 and m+2 are all divisible by a non‑trivial
 *            square
 * Range:     u8 n <= 5, u16 n <= 1231, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Return true iff x has a square divisor > 1.  */
static bool has_square(uint64_t x)
{
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(x, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] >= 2)
            return true;
    return false;
}

seq_t a(idx_t n)
{
    /* 0‑based array of computed terms; 1‑based indexing in OEIS. */
    static seq_t *terms = NULL;          /* array of seq_t elements */
    static size_t  cap   = 0;             /* allocated capacity */
    static idx_t   len   = 0;             /* number of terms currently stored */
    static uint64_t cur   = 0;             /* current integer being tested */

    if (n == 0)
        return 0;                         /* defined only for n ≥ 1 */

    /* Grow the array lazily when needed.  */
    if (cap == 0) {
        cap = 8;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms)
            abort();
    }

    /* Generate terms until we have at least n of them.  */
    while (len < n) {
        /* Find the next starting integer m such that
           m, m+1, m+2 are all divisible by some square.  */
        do {
            ++cur;
            if (cur > UINT64_MAX - 2)
                abort();                 /* safety check: unlikely to happen */
        } while (!(has_square(cur) && has_square(cur + 1) && has_square(cur + 2)));

        /* Store the found value.  Expand storage if necessary.  */
        if (len == cap) {
            size_t newcap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
            if (!tmp)
                abort();
            terms = tmp;
            cap = newcap;
        }
        terms[len++] = (seq_t)cur;        /* cast is safe for all supported seq_t */
    }

    return terms[n - 1];
}
