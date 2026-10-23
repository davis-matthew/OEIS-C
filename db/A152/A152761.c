/*
 * A152761  Sum of divisors of Catalan number A000108(n).
 * https://oeis.org/A152761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(Catalan(n)) (sum of divisors of Catalan number)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* compute the sequence value a(n) for given n (0‑based index) */
seq_t a(idx_t n)
{
    /* Catalan(0)=Catalan(1)=1 -> sigma(1)=1 */
    if (n <= 1) return (seq_t)1;

    /* ------------------------------------------------------------
       Build list of primes up to 2*n using a simple sieve.
       ------------------------------------------------------------ */
    idx_t limit = 2 * n;
    /* Allocate a boolean array.  Need explicit cast for C++ */
    bool *is_composite = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_composite) return (seq_t)0;   /* allocation failure */

    for (idx_t p = 2; p * p <= limit; ++p)
        if (!is_composite[p])
            for (idx_t j = p * p; j <= limit; j += p)
                is_composite[j] = true;

    /* Store primes in a dynamic array */
    size_t primes_cap = 1024;
    size_t primes_cnt = 0;
    uint64_t *primes = (uint64_t *)calloc(primes_cap, sizeof(uint64_t));
    if (!primes) { free(is_composite); return (seq_t)0; }

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) { free(is_composite); free(primes); return (seq_t)0; }
                primes = tmp;
            }
            primes[primes_cnt++] = (uint64_t)i;
        }
    }

    /* ------------------------------------------------------------
       For each prime compute its exponent in Catalan(n)
       ------------------------------------------------------------ */
    seq_t result = (seq_t)1;
    idx_t nn = n;
    idx_t tt = 2 * n;   /* 2n */

    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        uint64_t p = primes[idx];

        /* Legendre's formula for binom(2n,n) */
        uint64_t e_binom = 0;
        idx_t div2n = tt;
        idx_t divn = nn;
        while (div2n) {
            div2n /= p;
            divn /= p;
            e_binom += (uint64_t)div2n - 2 * (uint64_t)divn;
        }

        /* Subtract exponent of p in (n+1) */
        idx_t np1 = n + 1;
        uint64_t e_sub = 0;
        while (np1 % p == 0) {
            np1 /= p;
            e_sub++;
        }

        int e = (int)e_binom - (int)e_sub;
        if (e <= 0) continue;

        /* Compute σ(p^e) = (p^(e+1)-1)/(p-1) */
        seq_t power = (seq_t)1;
        for (int i = 0; i <= e; ++i)
            power *= (seq_t)p;          /* power = p^(e+1) */

        seq_t numerator   = power - (seq_t)1;
        seq_t denom       = (seq_t)(p - 1);
        seq_t component   = numerator / denom;

        result *= component;
    }

    free(is_composite);
    free(primes);
    return result;
}
