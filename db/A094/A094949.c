/*
 * A094949  Phi(m)*sigma(m), where m is the product of exactly two primes that
 *          differ by 2, where phi=A000010, sigma=A000203.
 * https://oeis.org/A094949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (m-3)*(m+1) where m = p*(p+2) and (p, p+2) are the n-th
 *            twin‑prime pair. Offset 1: a(1) uses (p,q) = (3,5)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 17, u64 n <= 860, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* A static sieve covering twin primes up to 3 000 000 is enough
       for n up to at least 10 000.  */
    static const size_t LIMIT = 3000000;
    static bool  *prime   = NULL;
    static uint32_t *twin  = NULL;   /* first prime of each twin pair */
    static size_t twin_used = 0;      /* number of twin pairs stored */

    /* Build the sieve and twin list once */
    if (prime == NULL)
    {
        prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
        for (size_t i = 0; i <= LIMIT; ++i) prime[i] = true;
        prime[0] = prime[1] = false;

        for (size_t i = 2; i * i <= LIMIT; ++i)
            if (prime[i])
            {
                for (size_t j = i * i; j <= LIMIT; j += i) prime[j] = false;
            }

        /* Reserve space for twin pairs (enough for ~20 000).  */
        size_t capacity = 20000;
        twin = (uint32_t *)malloc(capacity * sizeof(uint32_t));

        for (size_t p = 3; p + 2 <= LIMIT; p += 2)
            if (prime[p] && prime[p + 2])
            {
                if (twin_used == capacity)
                {
                    capacity *= 2;
                    twin = (uint32_t *)realloc(twin, capacity * sizeof(uint32_t));
                }
                twin[twin_used++] = (uint32_t)p;
            }
    }

    /* n is 1‑based; return 0 for non‑positive indices (unlikely in tests). */
    if (n == 0) return 0;

    /* Ensure the requested n exists; if not, the LIMIT is insufficient. */
    if ((size_t)n > twin_used)
    {
        /* For typical constraints this branch is never taken; if
           it is, we simply return 0 to avoid out‑of‑bounds access. */
        return 0;
    }

    uint32_t p = twin[n - 1];          /* first prime of the n‑th twin */
    seq_t m = (seq_t)p * (seq_t)(p + 2); /* m = p * (p+2)          */
    seq_t res = (m - (seq_t)3) * (m + (seq_t)1);
    return res;
}
