/*
 * A082454  a(n) = prime(n) + prime(n-1) - a(n-1) with a(0) = a(1) = 0.
 * https://oeis.org/A082454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + 2*(-1)^n (a(0)=a(1)=0)
 * Range:     u8 n <= 55, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Prime generation (1‑based).  Keeps a dynamic array of primes.          */
/* --------------------------------------------------------------------- */

static idx_t *primes = NULL;     /* primes[0] = 2, primes[1] = 3, ... */
static size_t primes_count = 0;  /* number of primes stored */
static idx_t next_candidate = 2; /* next odd number to test */

/* Grow the primes array so it can hold at least 'need' primes. */
static void ensure_primes(idx_t need)
{
    if (need <= primes_count)
        return;

    size_t newcap = primes_count ? primes_count * 2 : 64;
    while (newcap < (size_t)need)
        newcap *= 2;

    #if defined(__cplusplus)
    primes = reinterpret_cast<idx_t*>(realloc(primes, newcap * sizeof(idx_t)));
    #else
    primes = realloc(primes, newcap * sizeof(idx_t));
    #endif
    if (!primes) abort();

    /* Find new primes until we have the required count. */
    while (primes_count < (size_t)need)
    {
        if (next_candidate <= 2)
        {
            primes[primes_count++] = 2;
            next_candidate = 3;
            continue;
        }
        idx_t cand = next_candidate;
        bool is_prime = true;
        for (size_t i = 0; i < primes_count; ++i)
        {
            idx_t p = primes[i];
            if (p > cand / p)  /* p*p would overflow or p > sqrt(cand) */
                break;
            if (cand % p == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            primes[primes_count++] = cand;
        next_candidate += 2;   /* skip even numbers */
    }
}

/* Return the k‑th prime (1‑based).  k>=1. */
static seq_t nth_prime(idx_t k)
{
    if (k == 0) return (seq_t)0u;   /* undefined, but never used */
    ensure_primes(k);
    return (seq_t)primes[k-1];
}

/* --------------------------------------------------------------------- */
/* Sequence A082454                                                    */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 2)           /* a(0) = a(1) = 0 */
        return (seq_t)0u;

    seq_t p = nth_prime(n);   /* p = prime(n) */

    /* a(n) = prime(n) + 2*(-1)^n  ->  add 2 if n even, subtract 2 if n odd */
    return (n & 1) ? (p - 2) : (p + 2);
}
