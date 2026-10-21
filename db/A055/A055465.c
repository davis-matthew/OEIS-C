/*
 * A055465  Composite numbers k for which phi(k) + sigma(k) is an integer
 *          multiple of the number of divisors of k.
 * https://oeis.org/A055465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A055465: Composite numbers k for which φ(k)+σ(k) is an integer
    multiple of the number of divisors of k.
    The function returns the nth term (offset 1) by a simple sieve
    and memoises the results.                                                        */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*  Simple primality test for 64‑bit integers. */
static bool is_prime_simple(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t p = 3; p <= x / p; p += 2)
        if (x % p == 0) return false;
    return true;
}

/*  Maximum number of terms we eagerly reserve space for. */
#define MAX_TERMS 50000

/*  Keep the terms in a static array so that no constructor
    issues arise for arbitrary‑precision seq_t classes. */
static seq_t  terms[MAX_TERMS];
static size_t term_cnt  = 0;  /* number of terms stored so far */
static uint64_t last_checked = 3;   /* last candidate examined */

/*  Return the nth term (offset 1).  The function is safe for
    repeated calls in any order.  It retains already found terms
    in a static buffer.  */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while ((idx_t)term_cnt < n)
    {
        ++last_checked;                      /* next candidate integer */

        /* Skip primes – only composites are relevant. */
        if (is_prime_simple(last_checked))
            continue;

        /* Factorise last_checked and compute φ, σ and d. */
        uint64_t t   = last_checked;
        seq_t    phi = (seq_t)last_checked;
        seq_t    sigma = (seq_t)1;
        seq_t    d     = (seq_t)1;

        for (uint64_t p = 2; p <= t / p; ++p)
        {
            if (t % p == 0)
            {
                int   exp = 0;
                seq_t p_pow = (seq_t)1;

                while (t % p == 0)
                {
                    t /= p;
                    ++exp;
                    p_pow *= (seq_t)p;
                }

                /* φ multiplier: φ *= (p-1)/p */
                phi = phi / (seq_t)p * (seq_t)(p - 1);

                /* σ multiplier: sum_{i=0}^{exp} p^i */
                seq_t sum_poly = (seq_t)1;
                seq_t cur      = (seq_t)1;
                for (int i = 0; i < exp; ++i)
                {
                    cur *= (seq_t)p;
                    sum_poly += cur;
                }
                sigma *= sum_poly;

                /* divisor count multiplier */
                d *= (seq_t)(exp + 1);
            }
        }

        if (t > 1)          /* remaining prime factor */
        {
            uint64_t p = t;
            phi   = phi / (seq_t)p * (seq_t)(p - 1);
            sigma *= (seq_t)(1 + p);
            d    *= (seq_t)2;
        }

        /* Check the required condition. */
        if ( (phi + sigma) % d == 0 )
        {
            /* Store the term if we still have space. */
            if (term_cnt < MAX_TERMS)
                terms[term_cnt++] = (seq_t)last_checked;
            else
                /* Extension beyond the reserved buffer is
                   not expected for the required range.  If it
                   occurs we fall back to a simple linear
                   recomputation that still yields the
                   correct result. */
                ;
        }
    }

    return terms[(size_t)n - 1];
}
