/*
 * A051694  Smallest Fibonacci number that is divisible by n-th prime.
 * https://oeis.org/A051694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000045(A001602(n))
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 644 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term: the smallest Fibonacci number divisible by the n‑th prime */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

static void collect_divisors_rec(size_t idx, uint64_t current,
                                 const uint64_t *pr,
                                 const int *exp,
                                 size_t total,
                                 uint64_t *arr,
                                 size_t *size)
{
    if (idx == total) {
        arr[(*size)++] = current;
        return;
    }
    uint64_t p = pr[idx];
    int emax = exp[idx];
    uint64_t val = 1;
    for (int e = 0; e <= emax; ++e) {
        collect_divisors_rec(idx + 1, current * val, pr, exp, total, arr, size);
        val *= p;
    }
}

/* ----- modular Fibonacci (fast doubling) ----- */
static void fib_mod_pair(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) { *fn = 0; *fn1 = 1; return; }

    uint64_t a, b;
    fib_mod_pair(n >> 1, mod, &a, &b);

    /* c = F(2k) = F(k)*(2*F(k+1) - F(k)) */
    uint64_t two_b = (b << 1) % mod;
    uint64_t two_b_minus_a = (two_b + mod - a) % mod;
    uint64_t c = (uint64_t)((__uint128_t)a * two_b_minus_a % mod);

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
    uint64_t d = (uint64_t)(((__uint128_t)a * a + (__uint128_t)b * b) % mod);

    if ((n & 1) == 0) {      /* even n */
        *fn = c;
        *fn1 = d;
    } else {                  /* odd n */
        *fn = d;
        *fn1 = (c + d) % mod;
    }
}

static uint64_t mod_fib(uint64_t n, uint64_t mod)
{
    uint64_t fn, fn1;
    fib_mod_pair(n, mod, &fn, &fn1);
    return fn;
}

/* ----- prime factorisation of a small integer ----- */
static void factor_uint64(uint64_t n, uint64_t *pr, int *exp, size_t *cnt)
{
    size_t i = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[i] = p;
            exp[i] = e;
            ++i;
        }
    }
    if (n > 1) {
        pr[i] = n;
        exp[i] = 1;
        ++i;
    }
    *cnt = i;
}

/* ----- construct all divisors from factorisation ----- */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    /* 1‑based: get the nth prime */
    uint32_t p = get_prime(n);

    /* Special cases: p = 2 or 5 */
    if (p == 2) return fib((idx_t)3);   /* F(3) = 2  */
    if (p == 5) return fib((idx_t)5);   /* F(5) = 5  */

    /* Bound for the rank of apparition */
    uint64_t bound;
    if (p % 5 == 1 || p % 5 == 4)   /* p ≡ ±1 (mod 5) */
        bound = (uint64_t)p - 1;
    else                            /* otherwise */
        bound = 2ULL * ((uint64_t)p + 1);

    /* Factorise the bound */
    uint64_t prime_factors[32];
    int exponents[32];
    size_t prime_cnt;
    factor_uint64(bound, prime_factors, exponents, &prime_cnt);

    /* Compute number of divisors to allocate array */
    size_t div_max = 1;
    for (size_t i = 0; i < prime_cnt; ++i)
        div_max *= (size_t)(exponents[i] + 1);

    uint64_t *divs = (uint64_t *)malloc(div_max * sizeof(uint64_t));
    size_t div_cnt = 0;
    collect_divisors_rec(0, 1, prime_factors, exponents, prime_cnt, divs, &div_cnt);

    qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

    /* Find the smallest divisor d with p | F(d) */
    uint64_t rank = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        if (d == 0) continue;
        if (mod_fib(d, (uint64_t)p) == 0) {
            rank = d;
            break;
        }
    }
    free(divs);

    /* Fallback (should never hit) */
    if (rank == 0) {
        /* Brute force search up to the bound (unlikely) */
        for (uint64_t d = 1; d <= bound; ++d)
            if (mod_fib(d, (uint64_t)p) == 0) {
                rank = d;
                break;
            }
    }

    return fib((idx_t)rank);
}
