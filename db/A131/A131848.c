/*
 * A131848  Least nonnegative number which when added to the n-th semiprime
 *          gives a multiple of n.
 * https://oeis.org/A131848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = MIN{k >= 0 : n | (k + semiprime(n))}
 * Range:     u8 n <= 87, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A131848.  The function works for any
   order of calls, caching the semiprimes up to the required index. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*=== Cached data ==========================================================*/
static seq_t *semiprimes = NULL;   /* semiprimes[0] = first (4)          */
static size_t semiprimes_cnt = 0;   /* number currently stored            */
static idx_t  sieve_limit   = 1;    /* upper bound that has been sieved    */

/*=== Helper: extend semiprime cache ----------------------------------------*/
static void
ensure_semiprimes(idx_t need)
{
    if (need <= semiprimes_cnt)
        return;

    idx_t limit = (sieve_limit > 2) ? sieve_limit : 200;

    while (true) {
        /* Allocate and initialize SPF array.  Casting is needed for C++.     */
        uint32_t *spf = (uint32_t *)malloc(sizeof(uint32_t) * (limit + 1));
        if (!spf) {
            /* Out‑of‑memory – abort (no portable error handling required). */
            exit(EXIT_FAILURE);
        }
        for (idx_t i = 0; i <= limit; ++i)
            spf[i] = (uint32_t)i;

        /* Sieve for smallest prime factor. */
        for (idx_t i = 2; (i * (idx_t)i) <= limit; ++i) {
            if (spf[i] == (uint32_t)i) {                /* prime i         */
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == (uint32_t)j)
                        spf[j] = (uint32_t)i;
            }
        }

        /* Resize the semiprime array if necessary. */
        seq_t *new_arr = (seq_t *)realloc(semiprimes, need * sizeof(seq_t));
        if (!new_arr)
            exit(EXIT_FAILURE);
        semiprimes = new_arr;

        /* Scan numbers and collect semiprimes. */
        size_t cnt = 0;
        for (idx_t x = 2; x <= limit; ++x) {
            idx_t t = x;
            int f = 0;  /* number of prime factors counted so far      */
            while (t > 1 && f <= 2) {
                t /= spf[t];
                ++f;
            }
            if (f == 2) {                /* exactly two prime factors (semiprime)  */
                if (cnt < need)
                    semiprimes[cnt] = (seq_t)x;
                ++cnt;
                if (cnt >= need) {
                    free(spf);
                    semiprimes_cnt = cnt;
                    sieve_limit   = limit;
                    return;
                }
            }
        }

        /* Not enough semiprimes; double the bound and try again. */
        free(spf);
        limit *= 2;
    }
}

/*=== Public function -------------------------------------------------------*/
seq_t
a(idx_t n)
{
    if (n == 0)              /* undefined for n==0; return 0 as safety.    */
        return (seq_t)0;

    ensure_semiprimes(n);

    seq_t s  = semiprimes[n - 1];               /* nth semiprime          */
    seq_t nn = (seq_t)n;                        /* n as seq_t              */
    seq_t rem   = s % nn;                       /* remainder of s mod n    */
    seq_t k     = (nn - rem) % nn;              /* minimal k making n | (k+s) */

    return k;
}
