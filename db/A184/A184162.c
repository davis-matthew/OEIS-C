/*
 * A184162  Number of chains in the rooted tree with Matula-Goebel number n.
 * https://oeis.org/A184162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 1 * if n = prime(t) (t is the index of n in the prime
 *            list) then * a(n) = 1 + 2 * a(t) * if n = r * s (r,s ≥ 2) then *
 *            a(n) = a(r) + a(s) – 1 * * Computation is done by recomputing
 *            the required values * linearly up to the largest n requested.
 *            All work is * performed with integer arithmet...
 * Range:     u8 n <= 1417, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* small helper to grow an array */
static void *
grow_array(void *ptr, size_t *cap, size_t need, size_t elem_size)
{
    if (need <= *cap)
        return ptr;
    size_t new_cap = (*cap) ? (*cap) * 2 : 1;
    while (new_cap < need)
        new_cap++;
    void *tmp = realloc(ptr, new_cap * elem_size);
    if (!tmp && new_cap > 0)                /* allocation failed */
        exit(EXIT_FAILURE);
    *cap = new_cap;
    return tmp;
}

seq_t a(idx_t n)
{
    if (n == 0)              /* offset is 1; undefined below – return 0 */
        return (seq_t)0;

    /* static caches ----------------------------------------------------- */
    static seq_t  *value        = NULL;   /* a[1 .. N]            */
    static size_t val_cap      = 0;
    static size_t val_size     = 0;       /* highest index computed (N+1) */

    static uint32_t *spf          = NULL; /* smallest prime factor */
    static size_t   spf_cap       = 0;

    static uint32_t *prime_index  = NULL; /* index of a prime; 0 if composite */
    static size_t   prime_cap     = 0;

    static uint32_t *primes        = NULL; /* list of primes encountered   */
    static size_t   primes_cap     = 0;
    static size_t   primes_count   = 0;

    size_t need = (size_t)n + 1;          /* indices 0 .. n  */

    if (need <= val_size)
        return value[(size_t)n];

    /* enlarge all arrays to accommodate up to index n ------------------- */
    value        = (seq_t *)grow_array(value,       &val_cap,    need, sizeof(seq_t));
    spf          = (uint32_t*)grow_array(spf,        &spf_cap,    need, sizeof(uint32_t));
    prime_index  = (uint32_t*)grow_array(prime_index,&prime_cap,  need, sizeof(uint32_t));
    primes       = (uint32_t*)grow_array(primes,     &primes_cap,  need, sizeof(uint32_t));

    /* ------------------------------------------------------------------- */
    /* 1.  Sieve of Eratosthenes to fill spf (smallest prime factor).     */
    for (size_t i = 2; i < need; ++i)
        spf[i] = (uint32_t)i;                 /* assume prime */

    /* compute integer square root of n for loop bound */
    size_t limit = 0;
    while ((limit + 1) * (limit + 1) <= n)
        ++limit;

    for (size_t p = 2; p <= limit; ++p) {
        if (spf[p] == (uint32_t)p) {          /* p is prime */
            for (size_t j = p * p; j < need; j += p)
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)p;
        }
    }

    /* ------------------------------------------------------------------- */
    /* 2.  Build prime list and prime_index array.                         */
    primes_count = 0;
    for (size_t i = 2; i < need; ++i) {
        if (spf[i] == (uint32_t)i) {          /* i is prime */
            primes[primes_count++] = (uint32_t)i;
            prime_index[i] = (uint32_t)(primes_count); /* its order number */
        } else {
            prime_index[i] = 0;
        }
    }

    /* ------------------------------------------------------------------- */
    /* 3.  Compute the sequence values a(i).                               */
    value[1] = (seq_t)1;                     /* a(1) = 1 */

    for (size_t i = 2; i <= n; ++i) {
        if (spf[i] == (uint32_t)i) {         /* i is prime */
            size_t t = prime_index[i];       /* t < i */
            value[i] = (seq_t)1 + (seq_t)2 * value[t];
        } else {                             /* composite */
            uint32_t r = spf[i];
            uint32_t s = (uint32_t)(i / r);
            value[i] = value[r] + value[s] - (seq_t)1;
        }
    }

    val_size = need;                         /* cache up to n */
    return value[(size_t)n];
}
