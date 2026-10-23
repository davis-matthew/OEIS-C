/*
 * A165783  a(n) = A002326(n-1) + A000120(A165781(n-1)).
 * https://oeis.org/A165783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = L + popcount((2^L - 1)/(2*n-1)), with * L = minimal
 *            positive integer such that 2^L ≡ 1 (mod 2*n-1)
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize, popcount_div_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factorize.h"   /* factorize(uint64_t n, uint64_t primes[], int exps[]) */
#include "../utils/popcount_div_uint64.h"

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb);
}

/* Recursively generate all divisors of a number from its factorization. */
static void gen_divisors(const uint64_t *primes,
                         const int *exps,
                         int cnt,
                         int idx,
                         uint64_t cur,
                         uint64_t *out,
                         int *outcnt)
{
    if (idx == cnt) {
        out[*outcnt] = cur;
        (*outcnt)++;
        return;
    }
    uint64_t p = primes[idx];
    int e = exps[idx];
    uint64_t val = cur;
    for (int i = 0; i <= e; ++i) {
        gen_divisors(primes, exps, cnt, idx + 1, val, out, outcnt);
        val *= p;
    }
}

/* Fast modular exponentiation: compute 2^exp mod mod. */
static uint64_t powmod2(uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    uint64_t base = 2 % mod;
    while (exp) {
        if (exp & 1ULL)
            res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return res;
}

/* Return the multiplicative order of 2 modulo mod (mod > 1). */
static uint64_t multiplicative_order_mod2(uint64_t mod)
{
    if (mod == 1)
        return 1;                      /* special case for n = 1 */

    /* Compute φ(mod). */
    uint64_t primes[64];
    int exps[64];
    size_t pcnt = factorize(mod, primes, exps);

    uint64_t phi = mod;
    for (size_t i = 0; i < pcnt; ++i)
        phi = phi / primes[i] * (primes[i] - 1);

    /* Factor φ(mod). */
    uint64_t pp[64];
    int pexps[64];
    size_t cnt = factorize(phi, pp, pexps);

    /* Generate all divisors of φ(mod). */
    uint64_t divisors[4096];
    int dcnt = 0;
    gen_divisors(pp, pexps, cnt, 0, 1ULL, divisors, &dcnt);

    /* Sort divisors ascending. */
    qsort(divisors, (size_t)dcnt, sizeof(uint64_t), cmp_uint64);

    /* Find the smallest divisor d such that 2^d ≡ 1 (mod mod). */
    for (int i = 0; i < dcnt; ++i) {
        if (powmod2(divisors[i], mod) == 1ULL)
            return divisors[i];
    }

    /* If something goes wrong, fallback to φ(mod). */
    return phi;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined, safety */

    uint64_t mod = 2*(uint64_t)n - 1ULL;        /* 2*n - 1 */
    uint64_t L   = multiplicative_order_mod2(mod);
    uint64_t pop = popcount_div_uint64(L, mod);

    return (seq_t)L + (seq_t)pop;
}
