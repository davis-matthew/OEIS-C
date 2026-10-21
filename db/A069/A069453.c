/*
 * A069453  Lonely twin primes.
 * https://oeis.org/A069453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computes A069453 – lonely twin primes. a(2n-1) is the first
 *            prime of a lonely twin pair, a(2n) is the second prime of that
 *            pair. A prime p is *lonely* if p and p+2 are prime, and none of
 *            the following primes are part of any twin pair: • the prime
 *            immediately before p, • the prime immediately afte...
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static state to memoize between calls */
    static size_t    sieve_limit = 0;     /* current sieve upper bound      */
    static uint8_t  *is_comp     = NULL;  /* sieve array (0=prime,1=composite) */
    static uint32_t *primes      = NULL;  /* list of primes up to sieve_limit   */
    static size_t    primes_cnt  = 0;      /* number of primes                    */
    static seq_t   *lonely      = NULL;  /* stored lonely twin primes (size 2×count) */
    static size_t   lonely_cnt  = 0;      /* number of lonely twin primes stored  */

    /* produce enough lonely primes for the requested index */
    while (lonely_cnt < (size_t)n) {
        /* choose new limit, double each time (start at 2 million) */
        size_t new_limit = (sieve_limit == 0) ? 2000000ull : sieve_limit * 2ull;

        free(is_comp);
        free(primes);
        free(lonely);

        /* Sieve of Eratosthenes up to new_limit */
        is_comp = (uint8_t*)malloc(new_limit + 1);
        if (!is_comp) return 0;                /* out‑of‑memory fallback */
        memset(is_comp, 0, new_limit + 1);

        for (uint32_t i = 2; (uint64_t)i * i <= new_limit; ++i) {
            if (!is_comp[i]) {
                for (uint64_t j = (uint64_t)i * i; j <= new_limit; j += i)
                    is_comp[(size_t)j] = 1;
            }
        }

        /* collect primes into an array */
        size_t cnt = 0;
        for (uint32_t i = 2; i <= new_limit; ++i)
            if (!is_comp[i]) ++cnt;

        primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
        if (!primes) return 0;
        size_t k = 0;
        for (uint32_t i = 2; i <= new_limit; ++i)
            if (!is_comp[i]) primes[k++] = i;
        primes_cnt = cnt;

        /* extract lonely twin primes */
        seq_t *local = (seq_t*)malloc(primes_cnt * 2 * sizeof(seq_t));
        if (!local) return 0;
        size_t lcnt = 0;

        for (size_t j = 0; j + 1 < primes_cnt; ++j) {
            uint32_t p = primes[j];
            if (primes[j + 1] != p + 2) continue;   /* not a twin pair */

            int lonely_flag = 1;

            /* p-2 must not be prime */
            if (p >= 3 && !is_comp[p - 2]) lonely_flag = 0;

            /* preceding prime (if any) must not belong to a twin pair */
            if (j > 0) {
                uint32_t q = primes[j - 1];
                if (!is_comp[q - 2]) lonely_flag = 0;
            }

            /* following prime after the pair (if any) must not belong to a twin pair */
            if (j + 2 < primes_cnt) {
                uint32_t r = primes[j + 2];
                if (!is_comp[r - 2] || !is_comp[r + 2]) lonely_flag = 0;
            } else {
                /* if the pair is at the end, still need to check that p+4 is not prime */
                if (p + 4 <= new_limit && !is_comp[p + 4]) lonely_flag = 0;
            }

            if (lonely_flag) {
                local[lcnt++] = (seq_t)p;
                local[lcnt++] = (seq_t)(p + 2);
            }
        }

        lonely     = local;
        lonely_cnt = lcnt;
        sieve_limit = new_limit;
    }

    return lonely[n - 1];   /* 1‑based indexing */
}
