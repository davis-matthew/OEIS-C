/*
 * A171406  Solutions to the equation M(n) = 4 (M = Mertens's function
 *          A002321).
 * https://oeis.org/A171406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 898, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A171406: a(n) are the integers k such that M(k)=4,
   where M is the Mertens function.  This function computes
   solutions by incrementally sieving the Mertens function
   until it has found the n‑th solution. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *solutions = NULL;     /* array of found solutions          */
static size_t sol_count   = 0;       /* number of solutions stored        */
static size_t sol_cap     = 0;       /* allocated capacity of solutions    */
static idx_t  current_lim = 0;      /* largest limit that has been sieved */

/* ---------------------------------------------------------------------- */
/* Helper to build all solutions up to a given limit                     */
static void build_solutions(idx_t limit)
{
    size_t n = (size_t)limit + 1;

    /* mu array: initialise to 1 */
    signed char *mu = (signed char *)malloc(n);
    if (!mu) return;                  /* out of memory – unlikely in tests */
    memset(mu, 1, n);
    mu[0] = 0; mu[1] = 1;

    /* composite bitset: 1 bit per integer */
    size_t comp_sz = (limit >> 3) + 1;
    uint8_t *comp = (uint8_t *)calloc(comp_sz, 1);

    /* linear sieve to compute mu */
    for (idx_t i = 2; i <= limit; ++i) {
        if (!(comp[i >> 3] & (1U << (i & 7)))) {               /* prime i */
            /* toggle mu for multiples of i */
            for (idx_t j = i; j <= limit; j += i) {
                mu[j] = -mu[j];
                if (j > i) comp[j >> 3] |= (1U << (j & 7));
            }
            /* zero mu for multiples of i^2 (square factors) */
            idx_t sq = i * i;
            if (sq <= limit)
                for (idx_t j = sq; j <= limit; j += sq)
                    mu[j] = 0;
        }
    }

    /* accumulate Mertens function and collect solutions */
    seq_t M = 0;
    size_t cap = limit / 10 + 10;          /* initial capacity estimate */
    seq_t *newsol = (seq_t *)malloc(cap * sizeof(seq_t));
    size_t cnt = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        M += mu[i];
        if (M == 4) {
            if (cnt == cap) {
                cap *= 2;
                newsol = (seq_t *)realloc(newsol, cap * sizeof(seq_t));
            }
            newsol[cnt++] = (seq_t)i;
        }
    }

    /* clean up and store results */
    free(mu);
    free(comp);
    free(solutions);
    solutions = newsol;
    sol_count = cnt;
    sol_cap   = cap;
    current_lim = limit;
}

/* ---------------------------------------------------------------------- */
/* Ensure that we have computed at least `req` solutions                  */
static void ensure_solutions(idx_t req)
{
    if (req == 0) return;
    if (sol_count >= (size_t)req) return;

    idx_t lim = current_lim;
    if (lim == 0) lim = 1000;            /* start with a small limit   */
    while (sol_count < (size_t)req) {
        lim *= 2;                        /* increase limit exponentially */
        build_solutions(lim);
    }
}

/* ---------------------------------------------------------------------- */
/* Public function                                                    */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* not used by the tests */
    ensure_solutions(n);
    return solutions[n - 1];
}
