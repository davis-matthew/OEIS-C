/*
 * A092495  Least factorial multiple of n.
 * https://oeis.org/A092495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A002034(n))! (least factorial multiple of n)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/*---------------------------------------*/
/*  Prime sieve (once per program)       */
/*---------------------------------------*/
static size_t prime_cnt = 0;
static idx_t *prime_list = NULL;

static void init_primes(void)
{
    if (prime_cnt != 0) return;          /* already initialised */

    /* 1 000 000 gives 78 498 primes – plenty for n up to 10 000 */
    const size_t LIM = 1000000;
    char *is_prime = (char *)malloc(LIM + 1);
    memset(is_prime, 1, LIM + 1);
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= LIM; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= LIM; j += i)
                is_prime[j] = 0;

    for (size_t i = 2; i <= LIM; ++i)
        if (is_prime[i]) ++prime_cnt;

    prime_list = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    size_t idx = 0;
    for (size_t i = 2; i <= LIM; ++i)
        if (is_prime[i]) prime_list[idx++] = (idx_t)i;

    free(is_prime);
}

/*---------------------------------------*/
/*  Legendre: exponent of p in k!         */
/*---------------------------------------*/
static idx_t vp_factorial(idx_t k, idx_t p)
{
    idx_t sum = 0;
    idx_t div = p;

    while (div <= k) {
        sum += k / div;
        if (div > k / p) break;     /* prevent overflow in div * p */
        div *= p;
    }
    return sum;
}

/*---------------------------------------*/
/*  Minimal k with v_p(k!) >= e           */
/*---------------------------------------*/
static idx_t min_k_prime_power(idx_t p, idx_t e)
{
    idx_t low  = 1;
    idx_t high = p * e;      /* upper bound – safe for small e */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (vp_factorial(mid, p) >= e)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;
}

/*---------------------------------------*/
/*  Factorise n into primes (≤32 factors)*/
/*---------------------------------------*/
static void factorise(idx_t n,
                      idx_t factors[32], idx_t exps[32], size_t *cnt)
{
    *cnt = 0;
    idx_t temp = n;

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i) {
        idx_t p = prime_list[i];
        if (temp % p == 0) {
            size_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            factors[*cnt] = p;
            exps[*cnt]   = e;
            ++(*cnt);
        }
    }
    if (temp > 1) {                /* remaining prime factor */
        factors[*cnt] = temp;
        exps[*cnt]    = 1;
        ++(*cnt);
    }
}

/*---------------------------------------*/
/*  Main function: a(n)                   */
/*---------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* undefined, return 1 as safe value */

    init_primes();                 /* ensure primes are ready */

    /* factor n */
    idx_t f[32], e[32];
    size_t nf;
    factorise(n, f, e, &nf);

    /* find minimal k such that k! divisible by n */
    idx_t k = 1;
    for (size_t i = 0; i < nf; ++i) {
        idx_t kk = min_k_prime_power(f[i], e[i]);
        if (kk > k) k = kk;
    }

    /* compute k! in seq_t */
    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= k; ++i)
        result *= (seq_t)i;

    return result;
}
