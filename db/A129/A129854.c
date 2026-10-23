/*
 * A129854  Decimal Gray code ordering of the semiprimes.
 * https://oeis.org/A129854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A001358(1 + A003188(n))
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A003188(n) is the binary reflected Gray code of n, interpreted as a decimal integer. */

/* Compute the nth term of OEIS A129854: */
/* a(n) = the (1 + Gray(n))-th semiprime. */
/* The function maintains a cache of the first REQUIRED semiprimes, computed once on first call. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Number of semiprimes we pre‑compute to cover n up to about 10 000. */
    static const size_t REQUIRED = 20000;

    static int initialized = 0;     /* one‑time initialization flag   */
    static size_t semicap = 0;      /* number of semiprimes stored   */
    static seq_t *semiprimes = NULL;/* array of semiprimes           */

    /* One‑time precomputation of semiprimes. */
    if (!initialized)
    {
        /* Upper bound for sieve. 1 000 000 is safely enough for 20 000 semiprimes. */
        const size_t LIMIT = 1000000;

        /* Smallest prime factor table for all numbers up to LIMIT. */
        uint32_t *spf = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
        if (!spf) { return (seq_t)0; }  /* allocation failure, return 0 */

        for (size_t i = 0; i <= LIMIT; ++i) spf[i] = (uint32_t)i;
        for (size_t i = 2; i * i <= LIMIT; ++i)
        {
            if (spf[i] == (uint32_t)i)                /* i is prime */
            {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == (uint32_t)j)
                        spf[j] = (uint32_t)i;
            }
        }

        /* Allocate space for semiprimes. */
        semicap = REQUIRED;
        semiprimes = (seq_t*)malloc(semicap * sizeof(seq_t));
        if (!semiprimes) { free(spf); return (seq_t)0; }

        /* Scan numbers to collect semiprimes. */
        size_t count = 0;
        for (size_t x = 4; x <= LIMIT && count < semicap; ++x)
        {
            size_t temp = x;
            unsigned int factors = 0;
            while (temp > 1 && factors <= 2)
            {
                temp /= spf[temp];
                ++factors;
            }
            if (factors == 2)
                semiprimes[count++] = (seq_t)x;
        }

        /* Free the SPF table. */
        free(spf);

        /* Adjust semicap to actual count (may be slightly less than REQUIRED). */
        semicap = count;
        initialized = 1;
    }

    /* Compute Gray(n). */
    idx_t gray = n ^ (n >> 1);
    idx_t k = gray + 1;  /* 1‑based index into semiprime list */

    if (k == 0 || (size_t)k > semicap)
        return (seq_t)0;  /* out of bounds – should not happen for n ≤ 10 000 */

    return semiprimes[(size_t)(k - 1)];
}
