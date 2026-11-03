/*
 * A157132  Factorial of primes divided by prime numbers' respective places in
 *          the sequence of primes.
 * https://oeis.org/A157132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n)!/n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS entry defines a(n) as the factorial of the n‑th prime
   divided by n.  The utilities provided by the build supply
   get_prime(k) – which returns the k‑th prime as a uint32_t – and
   factorial(m) – which returns m! as a seq_t.  We therefore only
   need to call these helpers and perform the final integer
   division.  No mutable state or memoisation is required because
   `factorial` is defined to compute the factorial directly in a
   correct and efficient way. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* Valid indices are 1‑based.  Any call with n==0 is out of
       range for the OEIS specification; return 0 to remain
       well‑defined. */
    if (n == 0)
        return (seq_t)0;

    /* Obtain the n‑th prime.  It fits in a uint32_t, but we cast
       to seq_t only after the factorial is computed. */
    uint32_t p = get_prime(n);

    /* Compute factorial of the prime.  factorial accepts an
       idx_t (uint64_t) so casting from uint32_t is safe. */
    seq_t f = factorial((idx_t)p);

    /* Divide by n (canonical cast to seq_t). */
    return f / (seq_t)n;
}
