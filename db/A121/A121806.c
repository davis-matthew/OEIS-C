/*
 * A121806  a(n) = f(prime(n*2+1) mod 3, prime(n*2+2) mod 3) where f(1,1) = 3,
 *          f(1,2) = 1, f(2,1) = 2, f(2,2) = 4.
 * https://oeis.org/A121806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = f(prime(2n+1) mod 3, prime(2n+2) mod 3) where f(1,1) = 3,
 *            f(1,2) = 1, f(2,1) = 2, f(2,2) = 4
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/sieve_primes.h"

/* Static storage for a prime table.  primes_tbl[k] = (k+1)-th prime (0‑based). */
static idx_t *primes_tbl = NULL;     /* array of primes in the normal 0‑based order */
static size_t  primes_cnt  = 0;      /* number of primes in primes_tbl */
static idx_t  primes_limit = 0;     /* largest value up to which the table was sieved */

/* Ensure that primes_tbl contains at least `required` primes (0‑based).  */
static void ensure_primes(idx_t required)
{
    /* If we already have enough primes, nothing to do. */
    if (required < primes_cnt && primes_tbl != NULL)
        return;

    /* Estimate an upper bound on the value of the required‑th prime.
       For n ≥ 6,  p_n ≤ n (log n + log log n).  */
    idx_t new_limit;
    if (required <= 6)
        new_limit = 15ULL;                            /* small hand‑picked limit */
    else
    {
        double n   = (double)required;
        double val = n * (log(n) + log(log(n))) + 10.0;   /* safety margin */
        new_limit = (idx_t)val;
    }

    /* Never reduce the current limit. */
    if (new_limit < primes_limit)
        new_limit = primes_limit;

    /* Allocate a new table by sieving up to new_limit.  */
    if (primes_tbl != NULL)
        free(primes_tbl);
    sieve_primes(new_limit, &primes_tbl, &primes_cnt);
    primes_limit = new_limit;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  The behaviour for n == 0 is
       unspecified; we return 0 for safety. */
    if (n == 0)
        return 0;

    /* Indices of the required primes in the 0‑based array.  */
    idx_t idx1 = 2 * n;          /* (2n+1)-th prime → index 2n */
    idx_t idx2 = idx1 + 1;       /* (2n+2)-th prime → index 2n+1 */

    /* Make sure the table contains at least these primes. */
    ensure_primes(idx2 + 1);

    /* Fetch the primes. */
    idx_t p1 = primes_tbl[idx1];
    idx_t p2 = primes_tbl[idx2];

    int r1 = (int)(p1 % 3);
    int r2 = (int)(p2 % 3);

    /* Map the pair (r1, r2) to the required output.  The residues are
       either 1 or 2 for primes greater than 3. */
    seq_t result;
    if (r1 == 1 && r2 == 1)
        result = 3;
    else if (r1 == 1 && r2 == 2)
        result = 1;
    else if (r1 == 2 && r2 == 1)
        result = 2;
    else                     /* r1 == 2 && r2 == 2 */
        result = 4;

    return result;
}
