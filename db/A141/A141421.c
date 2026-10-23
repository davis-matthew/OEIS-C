/*
 * A141421  First bisection of A091137.
 * https://oeis.org/A141421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = a(n-1) * r(n) for n ≥ 1, where r(n) = 2^2 * 3 *
 *            ∏{p odd prime > 3 : (p-1)/2 divides n} p. Equivalently, a(n) =
 *            ∏_{k=1}^{n} r(k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the OEIS sequence A141421 (first bisection of A091137)                */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static idx_t max_n = 0;                     /* largest n already computed   */
    static seq_t *factor = NULL;                /* r(k) values                   */
    static seq_t *value  = NULL;                /* a(k) values                   */

    if (n == (idx_t)0)                          /* a(0) = 1                      */
        return (seq_t)1;

    /* if we need to extend our tables, recompute everything up to n              */
    if (n > max_n)
    {
        idx_t i, j, p;
        idx_t limit = 2*n + 1;                  /* upper bound for primes        */

        /* allocate / resize arrays */
        factor = (seq_t *)realloc(factor, (n+1) * sizeof(seq_t));
        value  = (seq_t *)realloc(value , (n+1) * sizeof(seq_t));

        /* ---------- prime sieve up to 2*n+1 --------------------------------- */
        uint8_t *is_prime = (uint8_t *)malloc((limit+1) * sizeof(uint8_t));
        for (i = 0; i <= limit; ++i) is_prime[i] = (uint8_t)1;
        is_prime[0] = is_prime[1] = (uint8_t)0;
        for (i = 2; i*i <= limit; ++i)
            if (is_prime[i])
                for (j = i*i; j <= limit; j += i)
                    is_prime[j] = (uint8_t)0;

        /* ---------- compute factor[k] = r(k) --------------------------------- */
        for (i = 1; i <= n; ++i)
            factor[i] = (seq_t)12;               /* 2^2 * 3 */

        /*  For each odd prime p > 3, multiply all multiples of L = (p-1)/2   */
        for (p = 5; p <= limit; p += 2)
            if (is_prime[p])
            {
                idx_t L = (p-1) / 2;
                if (L == 0 || L > n) continue;
                for (i = L; i <= n; i += L)
                    factor[i] *= (seq_t)p;
            }

        /* ---------- compute the sequence values ------------------------------ */
        value[0] = (seq_t)1;
        for (i = 1; i <= n; ++i)
            value[i] = value[i-1] * factor[i];

        /* free temporary prime array */
        free(is_prime);
        max_n = n;
    }

    return value[n];
}
