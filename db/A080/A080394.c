/*
 * A080394  Numbers k such that binomial(k, floor(k/2)) is divisible by k^2.
 * https://oeis.org/A080394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1737, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A080394
   a(n) = the n-th positive integer k such that C(k, floor(k/2)) is divisible by k². */

/* This implementation iteratively discovers the required integers.
   It uses a simple prime sieve (up to a fixed limit) and Legendre’s
   formula to check the divisibility condition.  A static cache stores
   the sequence values so that a(n) can be answered in any order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for primes */
static uint32_t *primes = NULL;
static size_t prime_cnt = 0;

/* generate all primes up to LIMIT once */
static void generate_primes(void)
{
    if (prime_cnt != 0) return;                 /* already done */
    const idx_t LIMIT = 2000000;                /* ample for k up to 10⁹ */
    bool *is_comp = (bool *)malloc((size_t)LIMIT + 1);
    if (!is_comp) abort();
    for (size_t i = 0; i <= (size_t)LIMIT; ++i) is_comp[i] = false;
    is_comp[0] = is_comp[1] = true;
    for (size_t i = 2; (idx_t)i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (size_t j = (size_t)i * i; j <= (size_t)LIMIT; j += i)
                is_comp[j] = true;
    /* count primes */
    for (size_t i = 2; i <= (size_t)LIMIT; ++i)
        if (!is_comp[i]) ++prime_cnt;
    primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes) abort();
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)LIMIT; ++i)
        if (!is_comp[i]) primes[idx++] = (uint32_t)i;
    free(is_comp);
}

/* exponent of prime p in n! (Legendre's formula) */
static idx_t vp_factorial(idx_t n, idx_t p)
{
    idx_t v = 0;
    for (idx_t d = p; d <= n; d *= p)
    {
        v += n / d;
        if (d > n / p) break;   /* avoid overflow */
    }
    return v;
}

/* test whether k satisfies the property */
static bool is_good_k(idx_t k)
{
    if (k <= 1) return true;            /* 1 is trivial */
    idx_t m   = k >> 1;                  /* floor(k/2) */
    idx_t o   = k - m;                   /* ceil(k/2)  */
    idx_t tmp = k;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint32_t p = primes[i];
        if ((idx_t)p * (idx_t)p > tmp) break;
        if (tmp % p != 0) continue;
        idx_t e = 0;
        while (tmp % p == 0) { tmp /= p; ++e; }
        idx_t vp = vp_factorial(k, p) - vp_factorial(m, p) - vp_factorial(o, p);
        if (vp < 2 * e) return false;
    }
    if (tmp > 1)                        /* remaining prime factor > sqrt(k) */
    {
        idx_t p = tmp;
        idx_t vp = vp_factorial(k, p) - vp_factorial(m, p) - vp_factorial(o, p);
        if (vp < 2) return false;       /* exponent in k is 1 */
    }
    return true;
}

/* cache for the sequence */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static idx_t next_k = 1;                /* next k to test */

/* public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1: n>=1 in terms of OEIS */
    generate_primes();                   /* ensure prime list ready */
    while (seq_len < (size_t)n)
    {
        if (is_good_k(next_k))
        {
            if (seq_len == seq_cap)
            {
                size_t new_cap = seq_cap ? seq_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return seq[n - 1];
}
