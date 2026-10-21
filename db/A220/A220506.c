/*
 * A220506  Number of primes <= n-th quarter-square.
 * https://oeis.org/A220506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = π(⌊n^2/4⌋) (number of primes ≤ the n‑th quarter‑square)
 * Range:     u8 n <= 31, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /*  Static arrays for a simple incremental sieve of Eratosthenes  */
    static uint8_t  *is_prime = NULL;   /* 1 = prime candidate, 0 = composite */
    static uint32_t *pi_pref  = NULL;   /* prefix sums of primes (π(k))   */
    static size_t   sieve_lim = 0;      /* current maximum index included */

    /*  Compute the n‑th quarter‑square : floor(n^2/4)  */
    seq_t nn  = (seq_t)n;
    seq_t qs  = (nn * nn) / 4;          /* integer division gives floor */
    /*  qs fits into size_t for the intended input sizes (≤ 10 000)  */
    size_t limit = (size_t)qs;

    /*  a(n)=0 for n=1 (qs=0) – no sieve needed  */
    if (limit == 0)
        return (seq_t)0;

    /*  If the transient sieve is too small, grow it  */
    if (limit > sieve_lim)
    {
        /*  Allocate temporary arrays  */
        uint8_t  *new_is = (uint8_t *)malloc(limit + 1);
        uint32_t *new_pi = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));

        if (!new_is || !new_pi)        /* allocation failure – fall back */
            return (seq_t)0;

        /*  Initialise all as prime candidates  */
        for (size_t i = 0; i <= limit; ++i)
            new_is[i] = 1;
        new_is[0] = new_is[1] = 0;

        /*  Sieve of Eratosthenes  */
        for (size_t p = 2; p * p <= limit; ++p)
        {
            if (new_is[p])
            {
                for (size_t j = p * p; j <= limit; j += p)
                    new_is[j] = 0;
            }
        }

        /*  Compute prefix sum of primes  */
        new_pi[0] = 0;
        for (size_t i = 1; i <= limit; ++i)
            new_pi[i] = new_pi[i-1] + new_is[i];

        /*  Free old data and store the new tables  */
        if (is_prime) free(is_prime);
        if (pi_pref)  free(pi_pref);

        is_prime = new_is;
        pi_pref  = new_pi;
        sieve_lim = limit;
    }

    /*  At this point pi_pref[limit] is valid  */
    return (seq_t)pi_pref[limit];
}
