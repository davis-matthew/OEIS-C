/*
 * A025532  a(n) is the sum of exponents in the prime factorization of
 *          lcm{C(n,0), C(n,1), ..., C(n,n)}.
 * https://oeis.org/A025532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p ≤ n+1} ⌊log_p(n+1)⌋ – Ω(n+1)
 * Range:     u8 n <= 1445, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the sum of exponents in the prime factorization of the least common
    multiple of the binomial coefficients C(n,0), …, C(n,n).  */

#include <stdint.h>
#include <stdlib.h>

/* Cached list of primes – kept between calls.  */
static uint32_t *primes      = NULL;   /* array of primes */
static size_t   primes_size  = 0;      /* allocated capacity */
static size_t   primes_count = 0;      /* number of primes stored */
static idx_t    primes_limit = 1;      /* largest number we have sieved to */

/* Extend the prime list up to 'limit' (inclusive).  */
static void
ensure_primes (idx_t limit)
{
    if (limit <= primes_limit) return;          /* already have enough */

    /* Grow the prime array if necessary.  */
    if (primes_count == primes_size) {
        size_t new_size = primes_size ? primes_size * 2 : 8;
        uint32_t *tmp = (uint32_t *)realloc (primes,
                                            new_size * sizeof (uint32_t));
        if (tmp == NULL) abort ();              /* allocation failure */
        primes      = tmp;
        primes_size = new_size;
    }

    /* Simple trial‑division sieve from primes_limit+1 up to limit.  */
    for (idx_t cand = primes_limit + 1; cand <= limit; ++cand) {
        int is_prime = 1;
        for (size_t i = 0; i < primes_count; ++i) {
            uint32_t p = primes[i];
            if ((idx_t)p * (idx_t)p > cand) break;
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            if (primes_count == primes_size) {
                size_t new_size = primes_size * 2;
                uint32_t *tmp = (uint32_t *)realloc (primes,
                                                    new_size * sizeof (uint32_t));
                if (tmp == NULL) abort ();
                primes      = tmp;
                primes_size = new_size;
            }
            primes[primes_count++] = (uint32_t)cand;
        }
    }
    primes_limit = limit;
}

/* Main sequence function.  */
seq_t
a (idx_t n)
{
    idx_t m = n + 1;          /* the term needed in the formula */

    /* Ensure we have all primes up to m.  */
    ensure_primes (m);

    /* Compute Σ_{p ≤ m} ⌊log_p(m)⌋.  */
    seq_t sumlog = 0;
    for (size_t i = 0; i < primes_count && primes[i] <= m; ++i) {
        idx_t p = primes[i];
        idx_t t = m;
        size_t e = 0;
        while (t >= p) {
            t /= p;
            ++e;
        }
        sumlog += (seq_t)e;
    }

    /* Compute Ω(m) – the total exponent sum in the prime factorization of m.
       This is obtained by factoring m with the cached primes.  */
    seq_t omega = 0;
    idx_t temp  = m;
    for (size_t i = 0; i < primes_count &&
                     (idx_t)primes[i] * (idx_t)primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        while (temp % p == 0) {
            temp /= p;
            ++omega;
        }
    }
    if (temp > 1) ++omega;   /* remaining prime factor */

    return sumlog - omega;
}
