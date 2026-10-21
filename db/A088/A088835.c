/*
 * A088835  a(n) = lcm(A020639(n), A006530(n)).
 * https://oeis.org/A088835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(A020639(n), A006530(n)) * In practice a(n) equals the
 *            product of the smallest and the largest * prime divisors of n;
 *            if n is a power of a single prime this product * reduces to that
 *            prime itself. This matches all terms supplied * in the OEIS
 *            entry
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"

/*  Cache of primes used for factorisation   */
static idx_t *prime_list = NULL;   /* array of primes  */
static size_t prime_cnt  = 0;      /* number of primes stored   */
static idx_t  prime_cap  = 0;      /* maximum prime value stored */

/*  Build a prime list up to or exceeding |limit|  */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_cap) return;                 /* already have enough */
    idx_t *new_list;
    size_t new_cnt;
    build_primes(limit, &new_list, &new_cnt);        /* from utils package   */
    free(prime_list);
    prime_list = new_list;
    prime_cnt  = new_cnt;
    prime_cap  = limit;
}

/*  Smallest and largest prime divisor of n  */
static void prime_bounds(idx_t n, idx_t *minp, idx_t *maxp)
{
    *minp = 0;
    *maxp = 0;
    if (n < 2) return;

    /*  Ensure we have primes up to n (safe for factorisation)   */
    ensure_primes(n);

    idx_t rem = n;
    for (size_t i = 0; i < prime_cnt && (idx_t)prime_list[i] * (idx_t)prime_list[i] <= rem; ++i) {
        idx_t p = prime_list[i];
        if (rem % p == 0) {
            if (!*minp) *minp = p;
            while (rem % p == 0) rem /= p;
            if (p > *maxp) *maxp = p;
        }
    }
    if (rem > 1) {           /* rem is a prime greater than sqrt(n) */
        if (!*minp) *minp = rem;
        if (rem > *maxp) *maxp = rem;
    }
}

/*  Main sequence function  */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;     /* a(1)=1, a(0) undefined but return 1 */

    idx_t sp = 0;  /* smallest prime divisor  */
    idx_t lp = 0;  /* largest  prime divisor  */
    prime_bounds(n, &sp, &lp);

    if (!sp) return (seq_t)1;    /* should not happen */

    if (sp == lp)                /* n is a power of a prime */
        return (seq_t)sp;

    /*  product of smallest and largest prime divisor  */
    return (seq_t)sp * (seq_t)lp;
}
