/*
 * A144049  Number of different cycles of digits in the hexadecimal (base-16)
 *          expansions of 1/p, 2/p, ..., (p-1)/p where p = n-th prime
 *          different from 2.
 * https://oeis.org/A144049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = (p-1) / ord_{16}(p) * where p is the nth odd prime
 *            (p(1)=3, p(2)=5, …) * ord_{16}(p) is the multiplicative order of
 *            16 modulo p
 * Range:     u8 n <= 53, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   build_primes, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_primes.h"
#include "../utils/mod_pow.h"

static idx_t *prime_array = NULL;
static size_t prime_count = 0;      /* number of primes stored (including 2) */

/* Make sure prime_array contains at least indices 0 … need.
 * prime_array[0] == 2, prime_array[1] == 3, etc. */
static void ensure_primes(size_t need)
{
    while (prime_count <= need) {
        size_t limit = prime_count ? prime_count * 2 + 10 : 20;
        /* try to generate at least need+1 primes */
        for (;;) {
            idx_t *tmp;
            size_t cnt;
            build_primes((idx_t)limit, &tmp, &cnt);
            if (cnt > need) {
                if (prime_array)
                    free(prime_array);
                prime_array = tmp;
                prime_count = cnt;
                return;
            }
            free(tmp);
            limit *= 2;
        }
    }
}

/* Compute the multiplicative order of 16 modulo the odd prime p (>2). */
/* The order divides p-1, so we factor p-1 and try to reduce it. */
static uint64_t multiplicative_order_16(uint64_t p)
{
    uint64_t ord = p - 1;
    uint64_t m = ord;

    /* trial division factorisation of m */
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0)
                m /= d;          /* remove d completely from m */

            /* try to reduce ord by the prime factor d */
            while (ord % d == 0) {
                uint64_t cand = ord / d;
                if (mod_pow((seq_t)16, cand, (seq_t)p) == (seq_t)1)
                    ord = cand;
                else
                    break;
            }
        }
    }
    /* if m > 1, it is the remaining prime factor */
    if (m > 1) {
        while (ord % m == 0) {
            uint64_t cand = ord / m;
            if (mod_pow((seq_t)16, cand, (seq_t)p) == (seq_t)1)
                ord = cand;
            else
                break;
        }
    }
    return ord;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1; return 0 for safety */

    /* obtain the nth odd prime (index n, skipping 2) */
    ensure_primes((size_t)n);
    uint64_t p = prime_array[n];          /* nth odd prime */
    uint64_t order = multiplicative_order_16(p);

    return (seq_t)((p - 1) / order);
}
