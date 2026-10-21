/*
 * A070072  Number of distinct rectangles with integer sides <= n and
 *          squarefree area.
 * https://oeis.org/A070072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{i=1..n} ∑_{j=1..i} μ(i·j)^2 (squarefree area ⇔ both i
 *            and j are squarefree and gcd(i,j)=1)
 * Range:     u8 n <= 40, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Euclidean algorithm for unsigned indices */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Cached data for successive calls */
static idx_t max_n_seen = 0;          /* maximum n that has been processed */
static char *sqfree = NULL;           /* square‑free flags 1..max_n_seen */
static idx_t *sqprime = NULL;         /* list of square‑free integers ≤ max_n_seen */
static size_t sqprime_cap = 0;        /* capacity of sqprime array */
static size_t sqprime_size = 0;       /* current number of square‑free integers */

/* Build or extend the square‑free table and list up to n */
static void build_squarefree(idx_t n)
{
    if (n <= max_n_seen)
        return;

    /* Allocate new table of size n+1 (index 0 unused) */
    char *new_sqfree = (char *)calloc((size_t)(n + 1), sizeof(char));
    if (!new_sqfree)
        return;                       /* allocation failure → leave old data intact */

    /* Mark all positive integers as square‑free initially */
    for (idx_t i = 1; i <= n; ++i)
        new_sqfree[i] = 1;

    /* Remove multiples of squares */
    for (idx_t p = 2; p * p <= n; ++p)
    {
        idx_t sq = p * p;
        for (idx_t k = sq; k <= n; k += sq)
            new_sqfree[k] = 0;
    }

    /* Grow or allocate the list of square‑free integers */
    size_t estimated_cap = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (new_sqfree[i])
            ++estimated_cap;

    if (estimated_cap > sqprime_cap)
    {
        idx_t *new_sqprime = (idx_t *)realloc(sqprime, estimated_cap * sizeof(idx_t));
        if (!new_sqprime)
        {
            free(new_sqfree);
            return;                   /* allocation failure → keep old data */
        }
        sqprime = new_sqprime;
        sqprime_cap = estimated_cap;
    }

    /* Fill the list */
    size_t pos = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (new_sqfree[i])
            sqprime[pos++] = i;
    sqprime_size = pos;

    /* Replace old data */
    free(sqfree);
    sqfree = new_sqfree;
    max_n_seen = n;
}

/* Main sequence function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    build_squarefree(n);            /* ensures sqfree and sqprime up to n */

    seq_t answer = 0;

    /* Count coprime pairs (i,j) with i>=j and both square‑free */
    for (size_t ai = 0; ai < sqprime_size; ++ai)
    {
        idx_t i = sqprime[ai];
        for (size_t bi = 0; bi <= ai; ++bi)
        {
            idx_t j = sqprime[bi];
            if (gcd_idx(i, j) == 1)
                ++answer;
        }
    }

    return answer;
}
