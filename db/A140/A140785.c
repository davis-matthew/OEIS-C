/*
 * A140785  a(n) = the single integer k, where p(n) <= k <= p(n+1), that is
 *          divisible by (p(n+1)-p(n)+1), where p(n) is the n-th prime.
 * https://oeis.org/A140785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p(n+1)-p(n)+1) * floor(p(n+1)/(p(n+1)-p(n)+1))
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes.h"

static idx_t *prime_arr = NULL;   /* 0‑based array of primes */
static size_t prime_cnt  = 0;     /* number of primes stored */

/* Ensure that at least `need` primes (p[1]..p[need]) are available. */
static void
ensure_primes(size_t need)
{
    if (prime_cnt >= need) return;

    /* Estimate a limit guaranteeing at least `need` primes.
       For n ≥ 10, p_n < 2*n*log(n) < 20*n for small n, safe enough.    */
    size_t limit;
    if (need < 10)          limit = 30;
    else                    limit = need * 20;

    /* Keep trying with larger limits until we have enough primes.   */
    for (;;) {
        if (prime_arr) {        /* free any earlier allocation   */
            free(prime_arr);
            prime_arr = NULL;
        }
        build_primes((idx_t)limit, &prime_arr, &prime_cnt);
        if (prime_cnt >= need) break;
        limit *= 2;         /* double the limit and retry */
    }
}

seq_t
a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0, return 0 */

    /* We need p(n) and p(n+1). */
    ensure_primes((size_t)n + 1);

    idx_t pn  = prime_arr[(size_t)n - 1];   /* p(n)   */
    idx_t pn1 = prime_arr[(size_t)n];       /* p(n+1) */

    seq_t d = (seq_t)(pn1 - pn + 1);        /* d = gap + 1  */
    seq_t t = (seq_t)pn1 / d;              /* floor(pn1/d) */
    return d * t;
}
