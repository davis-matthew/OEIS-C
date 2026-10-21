/*
 * A219157  Number of prime pairs {p,q} with p>q and p-2,q+2 also prime such
 *          that p+(1+mod(-n,6))q=n if n is not congruent to 2 mod 6, and
 *          p-q=n and q<n/2 if n=2 (mod 6).
 * https://oeis.org/A219157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9838, u16 n <= 9838, u32 n <= 9838, u64 n <= 9838,
 *            u128 n <= 9838, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = number of prime pairs {p,q} with p>q,
   p-2 and q+2 also prime such that:
   • if n≢2 (mod 6),   p + (1+(-n mod 6))·q = n;
   • if n≡2 (mod 6),   p – q = n and q < n/2.
   The function works for all n≥1 in any call order. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/*-----------------------------------------------------------------------------*
 * Prime sieve helpers (static, re‑used across calls)
 *-----------------------------------------------------------------------------*/
static unsigned char *prime_is = NULL; /* 0/1 flag array */
static size_t prime_limit = 0;          /* current highest index in prime_is */

/* Extend the sieve up to at least `limit`.  The array is rebuilt if needed. */
static void ensure_prime_limit(size_t limit)
{
    if (limit <= prime_limit)
        return;

    unsigned char *new_arr = (unsigned char *)malloc(limit + 1);
    if (!new_arr) /* de‑allocation failure – not expected in tests */
        return;

    /* Initialise all entries to prime */
    for (size_t i = 0; i <= limit; ++i)
        new_arr[i] = 1;
    new_arr[0] = new_arr[1] = 0;

    /* Classic Sieve of Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i)
        if (new_arr[i])
            for (size_t j = i * i; j <= limit; j += i)
                new_arr[j] = 0;

    free(prime_is);
    prime_is = new_arr;
    prime_limit = limit;
}

/*-----------------------------------------------------------------------------*
 * Main sequence function
 *-----------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    const size_t nsize = (size_t)n;
    const size_t mod6  = nsize % 6;
    seq_t count = 0;

    /* Determine how far we need to extend the prime table */
    size_t needed;
    if (mod6 == 2) {                     /* p = q + n ≤ (n/2) + n = 3n/2 */
        needed = nsize * 3 / 2 + 10;
    } else {                            /* all p ≤ n for the formula case */
        needed = nsize + 10;
    }
    ensure_prime_limit(needed);

    if (mod6 != 2) {
        /* Coefficient c = 1 + ((-n) mod 6) */
        const size_t coeff = 1 + ((6 - (nsize % 6)) % 6);

        /* q < n/(c+1)  →  q ≤ floor((n-1)/(c+1)) */
        const size_t q_max = (nsize - 1) / (coeff + 1);

        for (size_t q = 3; q <= q_max; q += 2) {
            if (!prime_is[q])                    continue;
            if (q + 2 > prime_limit || !prime_is[q + 2]) continue;

            const size_t p = nsize - coeff * q;
            if (p <= q)                          continue;
            if (p > prime_limit || !prime_is[p]) continue;
            if (p - 2 > prime_limit || !prime_is[p - 2]) continue;

            ++count;
        }
    } else { /* mod6 == 2 case: p - q = n, q < n/2 */
        const size_t q_limit = (nsize - 1) / 2;
        for (size_t q = 3; q <= q_limit; q += 2) {
            if (!prime_is[q])                    continue;
            if (q + 2 > prime_limit || !prime_is[q + 2]) continue;

            const size_t p = q + nsize;
            if (p > prime_limit || !prime_is[p]) continue;
            if (p - 2 > prime_limit || !prime_is[p - 2]) continue;

            ++count;
        }
    }

    return count;
}
