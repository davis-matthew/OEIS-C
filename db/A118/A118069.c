/*
 * A118069  a(n) = smallest composite which is > n! and is coprime to n!.
 * https://oeis.org/A118069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite > n! that is coprime to n!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   is_prime_big, seq_gcd, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* util functions from the framework */
#include "../utils/seq_one.h"          /* seq_one() */
#include "../utils/seq_mul.h"          /* seq_mul(seq_t, seq_t) */
#include "../utils/seq_gcd.h"          /* seq_gcd(seq_t, seq_t) */
#include "../utils/is_prime_big.h"     /* is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* static cache of factorials */
    static seq_t *fact = NULL;       /* array of seq_t */
    static size_t  fact_cap = 0;     /* current allocated size */
    static idx_t   fact_max = 0;     /* highest factorial computed */

    /* ensure we have enough space (C and C++ compatible cast) */
    if (fact_cap <= n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        fact = tmp;
        fact_cap = new_cap;
    }

    /* first entry: 0! = 1 */
    if (fact_max == 0 && fact_cap > 0)
        fact[0] = seq_one();

    /* compute missing factorials */
    for (idx_t i = fact_max + 1; i <= n; ++i) {
        fact[i] = seq_mul(fact[i - 1], (seq_t)i);
    }
    fact_max = n;

    /* an early check: is n! + 1 composite? */
    seq_t one  = seq_one();
    seq_t cand = fact[n] + (seq_t)1;
    if (!is_prime_big(cand))
        return cand;

    /* otherwise search for the first coprime composite > n! */
    for (idx_t t = 2; ; ++t) {
        seq_t m = fact[n] + (seq_t)t;
        /* m must be coprime with n! → gcd == 1 */
        seq_t g = seq_gcd(fact[n], m);
        if (g != one)
            continue;               /* share a prime factor <= n */

        /* and m must be composite */
        if (!is_prime_big(m))
            return m;
    }
}
