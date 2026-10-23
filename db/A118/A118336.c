/*
 * A118336  a(n) = greatest multiple of (p(n+1) - p(n)) which is < p(n), where
 *          p(m) is the m-th prime.
 * https://oeis.org/A118336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p(n+1)-p(n))*floor(p(n)/(p(n+1)-p(n))) for n>=2, a(1) =
 *            1 where p(k) is the k‑th prime. The implementation builds the
 *            list of primes on demand and stores them in an array of uint64_t
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*--- Static storage for prime list ------------------------------------*/
static uint64_t *primes  = NULL;      /* 0‑based array of primes          */
static size_t   primes_cnt  = 0;      /* number of primes currently stored*/
static size_t   primes_cap  = 0;      /* current allocation capacity       */

/*--- Ensure that at least “need” primes are available ----------------*/
static void ensure_primes(idx_t need)
{
    if (primes_cnt >= (size_t)need)
        return;                      /* already sufficient */

    if (!primes) {                   /* first allocation          */
        primes_cap = 128;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) exit(EXIT_FAILURE);
    }

    static uint64_t candidate = 2;    /* persistent across calls    */

    while (primes_cnt < (size_t)need) {
        /* test candidate for primality */
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) {   /* grow storage      */
                primes_cap *= 2;
                primes = (uint64_t *)realloc(primes,
                                             primes_cap * sizeof(uint64_t));
                if (!primes) exit(EXIT_FAILURE);
            }
            primes[primes_cnt++] = candidate;
        }
        ++candidate;
    }
}

/*--- Main sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;              /* a(1) is fixed */

    /* need primes at indices n and n+1 */
    ensure_primes(n + 1);

    seq_t p_n    = (seq_t)primes[n - 1];   /* p(n)  */
    seq_t p_next = (seq_t)primes[n];       /* p(n+1) */
    seq_t gap    = p_next - p_n;           /* p(n+1)-p(n) */
    seq_t q      = p_n / gap;              /* floor division */
    return gap * q;                        /* greatest multiple < p(n) */
}
