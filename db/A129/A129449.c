/*
 * A129449  Expansion of psi(-x) * psi(-x^3) in powers of x where psi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A129449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 63, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A129449:
 *  a(n) = b(2*n + 1), where b is multiplicative with
 *  b(2^e)   = 0^e
 *  b(3^e)   = (-1)^e
 *  b(p^e)   = (1 + (-1)^e)/2          if p ≡ 5 or 11 (mod 12)
 *  b(p^e)   = e + 1                    if p ≡ 1 (mod 12)
 *  b(p^e)   = (-1)^e (e + 1)           if p ≡ 7 (mod 12)
 *
 *  The code computes 2*n+1, factors it using a small sieve
 *  and multiplies the local factors according to the rule above.
 */

#include <stdint.h>
#include <stdlib.h>

/* static storage for smallest prime factor sieve */
static uint32_t *spf = NULL;   /* smallest prime factor for every integer */
static size_t   spf_limit = 0; /* maximum n for which spf is initialized */

/* ------------------------------------------------------------------ */
/* Initialise the sieve up to at least `limit`.  The sieve fills `spf`
 * so that spf[m] is the smallest prime factor of m (spf[0]=spf[1]=0).
 * ------------------------------------------------------------------ */
static void
init_spf(size_t limit)
{
    if (limit <= spf_limit) return;          /* already big enough */

    /* reallocate the array */
    uint32_t *new_spf = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
    if (!new_spf) exit(EXIT_FAILURE);
    spf = new_spf;

    /* initialise */
    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;

    /* simple sieve of Eratosthenes to fill smallest prime factors */
    for (size_t i = 2; i <= limit; ++i)
    {
        if (spf[i] == 0)              /* i is prime */
        {
            spf[i] = (uint32_t)i;
            if (i <= limit / i)       /* avoid overflow of i*i */
            {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    spf_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  Multiplicative helper: compute b(p^e) for a given prime p and
 *  exponent e.  The result fits in seq_t because values are small
 *  for our range of n. ------------------------------------------ */
static seq_t
b_prime_power(uint32_t p, int e)
{
    if (p == 3)
    {
        return (e & 1) ? -1 : 1;
    }

    int rem = p % 12;
    if (rem == 1)          /* p ≡ 1 (mod 12) */
    {
        return (seq_t)(e + 1);
    }
    else if (rem == 5 || rem == 11)      /* p ≡ 5 or 11 (mod 12) */
    {
        return (e & 1) ? 0 : 1;
    }
    else if (rem == 7)      /* p ≡ 7 (mod 12) */
    {
        if (e & 1)
            return -(seq_t)(e + 1);
        else
            return (seq_t)(e + 1);
    }
    /* 2 cannot appear here (2*n+1 is odd), but for completeness we return 1 */
    return 1;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function. ---------------------------------------- */
seq_t
a(idx_t n)
{
    /* prepare sieve large enough for 2*n + 1 */
    size_t needed = (size_t)(2 * n + 1);
    if (needed > spf_limit)
        init_spf(needed);

    /* 2*n + 1 is always odd, so factorisation is safe */
    seq_t m = (seq_t)((uint64_t)2 * (uint64_t)n + 1);   /* cast to seq_t via uint64_t */

    if (m == 1) return 1;    /* b(1) = 1 */

    seq_t result = 1;

    while (m > 1)
    {
        uint32_t p = spf[(size_t)m];      /* smallest prime divisor */
        int e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }

        seq_t factor = b_prime_power(p, e);
        if (factor == 0)      /* any zero factor makes whole product zero */
        {
            return 0;
        }
        /* multiply safely; intermediate overflow unlikely for small n */
        result *= factor;
    }

    return result;
}
