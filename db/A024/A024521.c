/*
 * A024521  Erroneous version of A014284.
 * https://oeis.org/A024521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(1)=1, a(2)=2; for n≥3, a(n)=1+∑_{k=1}^{n-1} p_k where p_k is
 *            the k‑th prime. The sum of the first (n‑1) primes is
 *            pre‑computed and cached
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 20, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

extern seq_t a(idx_t n)
{
    /* Static state: sieve up to a fixed limit that contains many
       primes (enough for n up to ~10 000).  The limit 200 000
       yields 17984 primes, more than enough for the required range. */
    static const unsigned LIMIT = 200000u;
    static bool initialized = false;
    static seq_t *prefix = NULL;        /* prefix[i] = sum of first i primes */
    static size_t prime_cnt = 0;        /* number of primes found */

    /* One‑time sieve and prefix sum construction. */
    if (!initialized)
    {
        initialized = true;

        unsigned char *sieve = (unsigned char*)malloc(LIMIT + 1);
        if (!sieve) return 0;                 /* allocation failed – give up */

        for (unsigned i = 0; i <= LIMIT; ++i) sieve[i] = 1;
        sieve[0] = sieve[1] = 0;

        for (unsigned i = 2; i * i <= LIMIT; ++i)
            if (sieve[i])
                for (unsigned j = i * i; j <= LIMIT; j += i)
                    sieve[j] = 0;

        /* Count primes for allocation. */
        size_t cnt = 0;
        for (unsigned i = 2; i <= LIMIT; ++i)
            if (sieve[i]) ++cnt;
        prime_cnt = cnt;

        prefix = (seq_t*)malloc((prime_cnt + 1) * sizeof(seq_t));
        if (!prefix) { free(sieve); return 0; }

        prefix[0] = 0;          /* sum of zero primes is 0 */
        seq_t sum = 0;
        size_t idx = 1;
        for (unsigned i = 2; i <= LIMIT; ++i)
            if (sieve[i])
            {
                sum += (seq_t)i;   /* add prime to running sum  */
                prefix[idx++] = sum;
            }

        free(sieve);
    }

    if (n < 1) return 0;          /* undefined for indices below the offset */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if ((size_t)(n - 1) > prime_cnt) return 0;   /* safety check */

    /* For n ≥ 3: a(n) = 1 + sum of first (n‑1) primes. */
    return prefix[(size_t)(n - 1)] + (seq_t)1;
}
