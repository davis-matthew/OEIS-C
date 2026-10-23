/*
 * A015916  Numbers k such that sigma(k) + 10 = sigma(k+10).
 * https://oeis.org/A015916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k such that sigma(k) + 10 = sigma(k+10);
 *            a(1) is the first such k (3). The following terms are found by
 *            testing successive integers
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* -----------------   Parameters   ----------------- */
static const idx_t LIMIT = 5000000;           /* Sufficient for n <= 10000 */

/* -----------------   Lazy static state   ----------------- */
static bool initialized = false;

static idx_t *sigma = NULL;         /* sum of divisors for 0..LIMIT+10 */
static idx_t *terms = NULL;        /* collected k values */
static size_t termCount = 0;       /* number of terms collected so far */
static size_t termsCap = 0;        /* capacity of the terms array */
static idx_t nextTest = 3;         /* next k to evaluate, start after offset */

/* -------------------------------------------------------- */
/* Helper: Grow the terms array.                           */
static void grow_terms(void)
{
    size_t newCap = termsCap == 0 ? 16 : termsCap * 2;
    idx_t *tmp = (idx_t *)realloc(terms, newCap * sizeof(idx_t));
    if (!tmp) abort();
    terms = tmp;
    termsCap = newCap;
}

/* -------------------------------------------------------- */
/* Main function\ */
seq_t a(idx_t n)
{
    /* ---------------------------------------------------- */
    /* Initialise the sigma array only once.                */
    if (!initialized)
    {
        size_t sz = (size_t)(LIMIT + 11);          /* indices 0 .. LIMIT+10 */
        sigma = (idx_t *)malloc(sz * sizeof(idx_t));
        if (!sigma) abort();
        for (size_t i = 0; i < sz; ++i)
            sigma[i] = 0;

        for (idx_t i = 1; i <= LIMIT + 10; (i++))
        {
            for (idx_t j = i; j <= LIMIT + 10; j += i)
                sigma[j] += i;
        }

        terms = NULL;
        initialized = true;
    }

    /* ---------------------------------------------------- */
    /* Generate terms until we have at least n of them.     */
    while (termCount < (size_t)n)
    {
        if (nextTest > LIMIT)
        {
            /* In practice LIMIT=5e6 is more than enough for n<=10000. */
            abort();  /* guarantee correctness for required n */
        }

        if (sigma[nextTest] + 10 == sigma[nextTest + 10])
        {
            if (termCount >= termsCap)
                grow_terms();
            terms[termCount++] = nextTest;
        }
        ++nextTest;
    }

    /* ---------------------------------------------------- */
    /* Convert the stored idx_t value to the requested seq_t. */
    return (seq_t)terms[n - 1];
}
