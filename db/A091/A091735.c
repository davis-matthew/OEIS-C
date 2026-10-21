/*
 * A091735  Primes arising in the first row of array in A091734.
 * https://oeis.org/A091735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(A007310(n)) where A007310(n) = 3*n - ((n & 1) ? 2 :
 *            1)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

/* If compiled as C++, explicitly cast from void* using reinterpret_cast */
#ifdef __cplusplus
#define PTR_CAST(T, p) reinterpret_cast<T>(p)
#else
#define PTR_CAST(T, p) ((T)(p))
#endif

seq_t a(idx_t n)
{
    /* Compute the index k of the desired prime */
    idx_t k = (idx_t)(3 * n - ((n & 1) ? 2 : 1));

    /* Static state to persist primes across calls */
    static seq_t *prime_list   = NULL;   /* array of found primes           */
    static size_t prime_count  = 0;      /* number of primes stored         */
    static size_t prime_alloc  = 0;      /* capacity of prime_list          */
    static uint8_t *comp       = NULL;   /* composite flags array           */
    static size_t comp_alloc   = 0;      /* capacity of comp array          */
    static uint64_t sieve_limit = 0;     /* last used sieve upper bound     */

    /* If we already have enough primes, simply return the result */
    if (prime_count < k)
    {
        /* Estimate an upper bound for the k-th prime */
        double dk = (double)k;
        uint64_t bound;
        if (k < 6)            /* safe bound for first few primes */
            bound = 15;
        else
            bound = (uint64_t)(dk * (log(dk) + log(log(dk))) + 3.0);

        /* Ensure the bound is larger than the previous sieve limit */
        if (bound <= sieve_limit)
            bound = sieve_limit * 2 + 100;   /* safety margin */

        /* Resize composite array if necessary */
        if (comp_alloc < bound + 1)
        {
            uint8_t *new_comp = PTR_CAST(uint8_t *, realloc(comp, bound + 1));
            if (!new_comp) abort();          /* memory exhausted */
            comp = new_comp;
            comp_alloc = bound + 1;
        }
        /* Clear composite flags */
        for (size_t i = 0; i <= bound; ++i)
            comp[i] = 0;

        /* Resize prime list if necessary */
        if (prime_alloc < k)
        {
            size_t new_cap = prime_alloc > 0 ? prime_alloc : 16;
            while (new_cap < k)
                new_cap <<= 1;
            seq_t *new_list = PTR_CAST(seq_t *, realloc(prime_list, new_cap * sizeof(seq_t)));
            if (!new_list) abort();          /* memory exhausted */
            prime_list = new_list;
            prime_alloc = new_cap;
        }

        /* Perform a simple sieve up to 'bound' */
        prime_count = 0;
        for (uint64_t i = 2; i <= bound; ++i)
        {
            if (!comp[i])
            {
                prime_list[prime_count++] = (seq_t)i;
                if (i <= bound / i)
                {
                    for (uint64_t j = i * i; j <= bound; j += i)
                        comp[j] = 1;
                }
            }
        }
        sieve_limit = bound;
    }

    /* Return the k-th prime (arrays are 0‑based) */
    return prime_list[k - 1];
}
