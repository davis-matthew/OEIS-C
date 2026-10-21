/*
 * A081401  Pseudologarithm (A056239) of n!: a(n) = A056239(A000142(n)).
 * https://oeis.org/A081401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{prime p <= n} index(p) * exp_p(n!) , where index(p)
 *            is the 1‑based index of prime p in the list (2,3,5,7,…) and
 *            exp_p(n!) = sum_{i>=1} floor(n / p^i)
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A081401.  */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* a(1) = 0, below the offset */

    /* Allocate a simple sieve array up to n.  */
    size_t size = (size_t)n + 1;
    uint8_t *mark = (uint8_t*)malloc(size);
    if (!mark) return 0;           /* allocation failure – should not happen */

    memset(mark, 0, size);

    /* Classic sieve: mark composites.  */
    for (size_t p = 2; p * p <= (size_t)n; ++p)
        if (!mark[p])
            for (size_t m = p * p; m <= (size_t)n; m += p)
                mark[m] = 1;

    seq_t sum = 0;
    size_t idx = 0;  /* prime index, 1‑based; use size_t to hold count */

    /* Iterate over primes and add their contributions.  */
    for (size_t p = 2; p <= (size_t)n; ++p) {
        if (!mark[p]) {
            ++idx;                       /* current prime is the idx‑th prime */

            /* Compute exponent of p in n! using Legendre's formula.  */
            idx_t exp = 0;
            idx_t pp  = (idx_t)p;
            while (pp <= n) {
                exp += n / pp;
                if (pp > n / p) break;   /* prevent overflow of pp*p */
                pp *= p;
            }

            /* Add idx * exp to the running sum (converted to seq_t).  */
            sum += (seq_t)idx * (seq_t)exp;
        }
    }

    free(mark);
    return sum;
}
