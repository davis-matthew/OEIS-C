/*
 * A064148  Numbers k such that mu(k) = mu(k+1), where mu is the Möbius
 *          function (A008683).
 * https://oeis.org/A064148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = the n-th integer k such that Möbius(k) = Möbius(k+1)
 *            (OEIS A064148)
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation pre‑computes the Möbius function by a linear sieve and
   stores all qualifying k up to a growing limit.  Subsequent calls reuse the
   cached data; if the requested index exceeds the current cache it grows
   to the next power of two.  The algorithm is O(L) where L is the
   current pre‑computation limit.*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* types supplied by the build system */
 /* seq_t : the type used for sequence values */
 /* idx_t : an unsigned integer type (uint64_t) */

static seq_t  *terms   = NULL;      /* array of computed terms */
static idx_t   terms_count = 0;     /* number of terms stored */

static int8_t *mu      = NULL;      /* Möbius values */
static idx_t   mu_limit = 0;        /* current Möbius pre‑computation limit */

/* initial pre‑computation limit (sufficient for n ~ 10000) */
#define START_LIMIT 500000

/* forward declarations */
static void compute_mu(idx_t N);          /* compute μ(1)…μ(N+1) */
static void build_terms(idx_t N);         /* build all terms k≤N with μ(k)=μ(k+1) */
static void recompute(idx_t N);           /* rebuild everything up to N */

/***********************************************************************
 * linear sieve for the Möbius function
 **********************************************************************/
static void compute_mu(idx_t N)
{
    /* allocate/resize the μ array */
    mu = (int8_t *)realloc(mu, (N + 2) * sizeof(int8_t));   /* μ[0]..μ[N+1] */
    if (!mu) exit(1);

    /* auxiliary arrays for the sieve */
    int   *primes   = (int *)malloc(N * sizeof(int));
    bool  *is_comp  = (bool *)malloc((N + 2) * sizeof(bool));
    if (!primes || !is_comp) exit(1);

    mu[0] = 0;
    mu[1] = 1;
    size_t pcnt = 0;

    for (idx_t i = 2; i <= N + 1; ++i)
    {
        if (!is_comp[i])
        {
            primes[pcnt++] = (int)i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < pcnt; ++j)
        {
            idx_t p = primes[j];
            idx_t x = i * p;
            if (x > N + 1) break;
            is_comp[x] = true;
            if (i % p == 0)
            {
                mu[x] = 0;
                break;
            }
            else
            {
                mu[x] = -mu[i];
            }
        }
    }

    free(primes);
    free(is_comp);
    mu_limit = N + 1;
}

/***********************************************************************
 * build the sequence up to k = N
 **********************************************************************/
static void build_terms(idx_t N)
{
    /* allocate term array with capacity N (max N terms) */
    terms = (seq_t *)realloc(terms, (N + 1) * sizeof(seq_t));
    if (!terms) exit(1);

    terms_count = 0;
    for (idx_t k = 1; k <= N; ++k)
    {
        if (mu[k] == mu[k + 1])
        {
            terms[terms_count++] = (seq_t)k;
        }
    }
}

/***********************************************************************
 * initialise or enlarge the pre‑computed tables
 **********************************************************************/
static void recompute(idx_t N)
{
    /* free previous tables */
    free(mu);
    free(terms);
    mu = NULL;
    terms = NULL;

    /* compute μ up to N+1 and build terms up to N */
    compute_mu(N);
    build_terms(N);
}

/***********************************************************************
 * public function required by the framework
 **********************************************************************/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe */

    /* initialise if this is the first call */
    if (terms == NULL)
    {
        recompute(START_LIMIT);
    }

    /* grow the tables if the index is beyond the current cache */
    while (n > terms_count)
    {
        idx_t new_limit = mu_limit * 2;          /* double the current limit */
        recompute(new_limit);
    }

    return terms[n - 1];
}
