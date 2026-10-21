/*
 * A056627  a(n) = A056622(n!).
 * https://oeis.org/A056627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p ≤ n} p^{ e/2 - δ } where e = exponent of prime p in
 *            n! (Legendre's formula), δ = 1 if e is odd and e ≥ 3, otherwise
 *            δ = 0. (e/2 is integer division.)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 25, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Multiply by squaring.  Exponent is an idx_t. */
static seq_t ipow(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1)
            result *= base;
        exp >>= (idx_t)1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;

    /* Simple sieve up to n to get primes. */
    bool *mark = (bool*)calloc((size_t)(n + 1), sizeof(bool));
    if (!mark) return (seq_t)0;
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= (idx_t)n; ++i) {
        if (!mark[i]) {
            ++prime_cnt;
            if (i <= (idx_t)(n / i))
                for (idx_t j = i * i; j <= (idx_t)n; j += i)
                    mark[j] = true;
        }
    }

    /* Allocate array for primes. */
    uint64_t *primes = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!primes) {
        free(mark);
        return (seq_t)0;
    }
    size_t pc = 0;
    for (idx_t i = 2; i <= (idx_t)n; ++i)
        if (!mark[i])
            primes[pc++] = (uint64_t)i;
    free(mark);

    seq_t result = (seq_t)1;
    for (size_t idx = 0; idx < pc; ++idx) {
        idx_t p = (idx_t)primes[idx];
        /* Compute exponent of p in n! */
        idx_t e = 0;
        idx_t power = p;
        while (power <= n) {
            e += n / power;
            if (power > n / p) break;      /* prevent overflow */
            power *= p;
        }
        /* exponent for the product in a(n) */
        idx_t exp = e / (idx_t)2;
        if ((e & (idx_t)1) && e >= (idx_t)3)
            exp--;                         /* subtract one when e is odd ≥3 */

        if (exp > (idx_t)0)
            result *= ipow((seq_t)p, exp);
    }
    free(primes);
    return result;
}
