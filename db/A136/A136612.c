/*
 * A136612  a(n) = ((prime(n+3) + prime(n+1)) - (prime(n+2) + prime(n))).
 * https://oeis.org/A136612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (prime(n+3)+prime(n+1)) - (prime(n+2)+prime(n))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of two consecutive prime gaps: g_{n+1}+g_{n+2}  */
/* a(1)=3, a(2)=6, a(3)=4, ...                                         */

#include "../utils/sieve_first_n_primes.h"
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* undefined for offset 0 */

    /* static cache of the first K primes, where K >= n+3 */
    static uint32_t *primes = NULL;           /* array of first primes */
    static size_t prime_cnt = 0;              /* number of primes stored */

    idx_t need = n + 3;                       /* we need indices 1..n+3 */

    if (prime_cnt < (size_t)need) {
        /* regenerate primes up to the required count */
        if (primes) free(primes);
        sieve_first_n_primes(need, &primes, &prime_cnt);
    }

    /* convert 1-based indices to 0-based array positions */
    seq_t p0 = (seq_t)primes[n - 1];          /* prime(n)   */
    seq_t p1 = (seq_t)primes[n];              /* prime(n+1) */
    seq_t p2 = (seq_t)primes[n + 1];          /* prime(n+2) */
    seq_t p3 = (seq_t)primes[n + 2];          /* prime(n+3) */

    return p3 + p1 - p2 - p0;                 /* (p_{n+3}+p_{n+1})-(p_{n+2}+p_n) */
}
