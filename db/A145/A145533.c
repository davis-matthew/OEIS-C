/*
 * A145533  a(n) is the number of numbers removed in each step of
 *          Eratosthenes's sieve for 6!.
 * https://oeis.org/A145533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = #{m ∈ [2,720] | smallest prime factor of m is the n-th
 *            prime} – 1
 * Range:     u8 none, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (a(1)=359, a(2)=119, …) */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state, initialized once */
    static bool init      = false;
    static idx_t  limit   = 720;
    static idx_t  prime_count;
    static idx_t *primes  = NULL;           /* primes[0..prime_count-1] */
    static seq_t *count_by_prime = NULL;    /* count_by_prime[p] = number of m with lpf = p */

    if (!init)
    {
        /* ---- sieve for primes up to 720 and lpf array ---- */
        idx_t *lpf = (idx_t*)calloc(limit + 1, sizeof(idx_t));
        if (!lpf) return 0;                 /* allocation failure, return 0 */

        for (idx_t i = 2; i <= limit; ++i)
        {
            if (lpf[i] == 0)                /* i is prime */
            {
                lpf[i] = i;
                for (idx_t j = i; j <= limit; j += i)
                    if (lpf[j] == 0)
                        lpf[j] = i;
            }
        }

        /* ---- collect primes ---- */
        prime_count = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (lpf[i] == i)
                ++prime_count;

        primes = (idx_t*)malloc(prime_count * sizeof(idx_t));
        if (!primes) { free(lpf); return 0; }

        idx_t k = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (lpf[i] == i)
                primes[k++] = i;             /* primes[0] = 2, primes[1] = 3, … */

        /* ---- count numbers whose smallest prime factor is p ---- */
        count_by_prime = (seq_t*)calloc(limit + 1, sizeof(seq_t));
        if (!count_by_prime) { free(lpf); free(primes); return 0; }

        for (idx_t i = 2; i <= limit; ++i)
        {
            idx_t p = lpf[i];
            count_by_prime[p] = count_by_prime[p] + 1;
        }

        /* clean up */
        free(lpf);
        init = true;
    }

    /* ---------- answer for requested n ---------- */
    if (n == 0 || n > prime_count)
        return 0;

    idx_t p = primes[n - 1];          /* nth prime (1‑based index) */
    seq_t res = count_by_prime[p] - 1;/* subtract the prime itself */
    return res;
}
