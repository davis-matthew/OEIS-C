/*
 * A026420  a(n) = least positive integer > a(n-1) and not a(i)*a(j) for
 *          1<=i<j<=n, with initial terms 2,4.
 * https://oeis.org/A026420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by incremental search: find the least integer > a(n-1)
   that is not equal to a(i)*a(j) for any 1 <= i < j <= n (i.e. not a
   product of any two earlier terms).  The implementation keeps an
   exclusion bit‑array for all product values up to a fixed large
   limit, and generates terms on demand, caching them in a dynamic
   array.  The limit is chosen large enough to contain a(10000) for
   typical 64‑bit or arbitrary‑precision builds. */

#include <stdlib.h>
#include <string.h>

static size_t terms_capacity = 0;          /* capacity of terms array */
static size_t terms_size     = 0;          /* number of terms stored (index 1…size) */
static seq_t *terms          = NULL;      /* array of terms, 1‑indexed */

static size_t excl_capacity  = 0;          /* capacity of exclusion bit‑array */
static uint8_t *excluded     = NULL;       /* 0 = allowed, 1 = excluded */

static int initialized = 0;               /* flag whether init has run */

/* Initialise the data structures and the first two terms. */
static void init(void)
{
    /* A generous upper bound for the values that may appear in a(10000).
       20 000 000 is more than enough for the expected growth of the
       sequence, and still uses only about 20 MiB for the exclusion array. */
    excl_capacity = 20000000;
    excluded = (uint8_t *)malloc(excl_capacity);
    if (!excluded) abort();
    memset(excluded, 0, excl_capacity);

    terms_capacity = 128;
    terms = (seq_t *)malloc(terms_capacity * sizeof(seq_t));
    if (!terms) abort();

    terms[0] = 0;            /* unused */
    terms[1] = 2;            /* a(1) */
    terms[2] = 4;            /* a(2) */
    terms_size = 2;

    /* Mark the only product that is <= excl_capacity (2*4 = 8). */
    uint64_t prod = (uint64_t)terms[1] * (uint64_t)terms[2];
    if (prod < (uint64_t)excl_capacity) excluded[prod] = 1u;

    initialized = 1;
}

/* Ensure that the terms array can hold at least `need` indices. */
static void ensure_terms_capacity(size_t need)
{
    if (need <= terms_capacity) return;
    size_t new_cap = terms_capacity * 2;
    while (new_cap < need) new_cap *= 2;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!terms) abort();
    terms_capacity = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if ((size_t)n <= terms_size) return terms[(size_t)n];

    /* Generate terms up to n. */
    for (size_t k = terms_size + 1; k <= (size_t)n; ++k)
    {
        seq_t last = terms[terms_size];
        seq_t cand = last + 1;

        /* Scan forward until we find a number not excluded. */
        while (cand < (seq_t)excl_capacity && excluded[(size_t)cand])
            ++cand;

        /* cand is now the new term */
        ensure_terms_capacity(k + 1);
        terms[k] = cand;

        /* Record all products of the new term with previous terms. */
        for (size_t i = 1; i < k; ++i)
        {
            uint64_t prod = (uint64_t)terms[i] * (uint64_t)cand;
            if (prod < (uint64_t)excl_capacity)
                excluded[(size_t)prod] = 1u;
        }

        terms_size = k;
    }

    return terms[(size_t)n];
}
