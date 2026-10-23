/*
 * A045971  a(1)=8; if n = Product p_i^e_i, n > 1, then a(n) = Product
 *          p_{i+1}^{e_i+2}.
 * https://oeis.org/A045971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8 for n = 1; otherwise a(n) = Π( nextprime(p_i)^(e_i+2)
 *            ), where n = Π p_i^(e_i) (prime factorization)
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   factor_u64, next_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  External helper functions used:
    - factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
    - next_prime(uint64_t start)
    - seq_pow(seq_t base, idx_t exp) */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */
#include <stdint.h>   /* uint64_t */

#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)8;
    }

    uint64_t N = (uint64_t)n;
    /* Enough storage for all prime factors of a 64‑bit integer. */
    size_t maxcnt = 64;
    uint64_t *primes = (uint64_t *)malloc(maxcnt * sizeof(uint64_t));
    uint64_t *expo   = (uint64_t *)malloc(maxcnt * sizeof(uint64_t));
    if (!primes || !expo) {
        /* In a real program we would handle the allocation failure.
           Here we simply return 0 to satisfy the function contract. */
        return (seq_t)0;
    }

    size_t cnt = 0;
    factor_u64(N, primes, expo, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t next = next_prime(primes[i]);                 /* p_{i+1} */
        idx_t    exp  = (idx_t)(expo[i] + 2ULL);               /* e_i + 2   */
        seq_t term = seq_pow((seq_t)next, exp);                /* next^(exp) */
        result = result * term;
    }

    free(primes);
    free(expo);
    return result;
}
