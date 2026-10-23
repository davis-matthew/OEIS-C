/*
 * A014284  Partial sums of primes, if 1 is regarded as a prime (as it was
 *          until quite recently, see A008578).
 * https://oeis.org/A014284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A014284: Partial sums of primes, treating 1 as a prime.
   Formula (recurrence): a(1) = 1,
   a(n) = a(n‑1) + prime(n‑1)   for n ≥ 2.  */

/* The following function implements that recurrence by
   generating the first (n‑1) primes with a simple sieve.  */

#include <stdlib.h>   // malloc, calloc, free
#include <stdint.h>   // uint8_t
#include <math.h>     // log
#include <stddef.h>   // size_t

/* upper bound for the nth prime, for n ≥ 6:  p_n ≤ n (log n + log log n) */
static idx_t prime_upper_bound(idx_t n)
{
    if (n < 6) return 15;                /* safe upper bound for small n */
    double dn  = (double)n;
    double ub  = dn * (log(dn) + log(log(dn))) + 3.0;
    return (idx_t)ub;
}

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 for safety. */
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;

    idx_t need  = n - 1;                     /* number of primes needed */
    idx_t limit = prime_upper_bound(need);   /* sieve limit */

    /* Boolean array: 0 = potentially prime, 1 = composite. */
    size_t sz = (size_t)limit + 1;
    uint8_t *is_comp = (uint8_t*)calloc(sz, 1);   /* allocate and zero */
    if (!is_comp) return (seq_t)0; /* allocation failure; unlikely */

    seq_t sum   = (seq_t)1;        /* start with the “prime” 1 */
    idx_t count = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {            /* i is a prime */
            sum += (seq_t)i;          /* add to partial sum */
            ++count;
            if (count == need) break;

            /* Mark multiples of i as composite. */
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    is_comp[j] = 1;
                }
            }
        }
    }

    free(is_comp);
    return sum;
}
