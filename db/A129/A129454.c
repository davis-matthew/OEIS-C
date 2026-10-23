/*
 * A129454  a(n) = Product{i=1..n-1} Product{j=1..n-1} Product{k=1..n-1}
 *          gcd(i,j,k).
 * https://oeis.org/A129454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1}^{n-1}∏_{j=1}^{n-1}∏_{k=1}^{n-1} gcd(i,j,k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   build_primes, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (for n≤2, a(n)=1)  */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes.h"   /* provides build_primes() */
#include "../utils/seq_mul.h"        /* provides seq_mul() */

static idx_t *prime_list = NULL;    /* array of primes ≤ current limit   */
static size_t prime_cnt   = 0;      /* number of primes in prime_list     */
static idx_t prime_limit  = 0;      /* current maximum prime that may be needed */

/* Ensure that primes up to 'limit' are pre‑computed */
static void ensure_primes(idx_t limit) {
    if (limit <= prime_limit)
        return;
    /* Allocate new array of primes up to 'limit' */
    idx_t *new_list;
    size_t new_cnt;
    build_primes(limit, &new_list, &new_cnt);
    /* Free old array if it exists */
    if (prime_list)
        free(prime_list);
    prime_list = new_list;
    prime_cnt  = new_cnt;
    prime_limit= limit;
}

seq_t a(idx_t n) {
    if (n <= 2)          /* a(0)=a(1)=a(2)=1 */
        return (seq_t)1;

    idx_t m = n - 1;     /* use indices 1…m */
    ensure_primes(m);

    seq_t result = (seq_t)1;

    for (size_t ip = 0; ip < prime_cnt && prime_list[ip] <= m; ++ip) {
        idx_t p = prime_list[ip];
        /* exponent of prime p in a(n) */
        idx_t e = 0;
        idx_t t = m / p;
        while (t) {
            e += t * t * t;   /* add (floor(m / p^k))^3   */
            t /= p;
        }
        /* compute p^e using exponentiation by squaring */
        seq_t p_pow = (seq_t)1;
        seq_t base  = (seq_t)p;
        idx_t exp   = e;
        while (exp) {
            if (exp & 1)
                p_pow = seq_mul(p_pow, base);
            base = seq_mul(base, base);
            exp >>= 1;
        }
        result = seq_mul(result, p_pow);
    }
    return result;
}
