/*
 * A028422  Number of ways n can be properly factored.
 * https://oeis.org/A028422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (number of unordered factorizations of n into integers >
 *            1) - 1, i.e., OEIS A028422
 * Range:     u8 n <= 2879, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t count_fact(idx_t n, idx_t min_fac)
{
    /* Count unordered factorizations of n (≥2) into factors ≥ min_fac,
       including the trivial factorization consisting of n itself. */
    seq_t cnt = 1;              /* the factorization “n” itself */
    for (idx_t d = min_fac; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            idx_t e = n / d;
            /* Recursively factor e with the non‑decreasing constraint d. */
            cnt += count_fact(e, d);
        }
    }
    return cnt;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n < 2)            /* primes, 1 or 0 */
        return 0;
    return count_fact(n, 2) - 1;   /* exclude the “n”‑only factorization */
}
