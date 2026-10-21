/*
 * A070962  a(n) = Card{ k<=n | omega(k)!=omega(n) }, where omega(n) =
 *          A001221(n).
 * https://oeis.org/A070962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{ k ≤ n : ω(k) ≠ ω(n) }|, where ω is the number of
 *            distinct prime factors
 * Range:     u8 n <= 321, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for ω values */
static unsigned char *omega_tbl = NULL;   /* ω(i) for i = 0 … cache_cap-1 */
static idx_t cache_cap   = 0;             /* number of elements stored (≥ 1) */

/* Build ω table up to 'limit' inclusive.  Rebuilds the whole table
   if the current capacity is insufficient. */
static void ensure_omega_up_to(idx_t limit)
{
    if (cache_cap > 0 && cache_cap - 1 >= limit)
        return; /* already sufficient */

    const size_t new_cap = (size_t)(limit + 1);
    unsigned char *new_tbl = (unsigned char *)malloc(new_cap * sizeof(unsigned char));
    if (!new_tbl)
        exit(1); /* fatal allocation failure */

    /* initialise */
    for (idx_t i = 0; i <= limit; ++i)
        new_tbl[i] = 0;

    /* classic sieve: for each prime p, increment ω[j] for all multiples j */
    for (idx_t p = 2; p <= limit; ++p) {
        if (new_tbl[p] == 0) {          /* p is prime */
            for (idx_t j = p; j <= limit; j += p)
                new_tbl[j] += 1;
        }
    }

    /* replace old table */
    free(omega_tbl);
    omega_tbl = new_tbl;
    cache_cap = (idx_t)new_cap;
}

/* Return a(n) as defined in the OEIS entry. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* below offset, not defined */

    ensure_omega_up_to(n);

    unsigned char omega_n = omega_tbl[n];
    seq_t count = 0;

    for (idx_t k = 1; k <= n; ++k)
        if (omega_tbl[k] != omega_n)
            count += 1;

    return count;
}
