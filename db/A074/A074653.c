/*
 * A074653  1/n * sum over d|n of {mu(n/d) * (5d)! / d!^5}.
 * https://oeis.org/A074653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/n * Σ_{d|n} μ(n/d) * (5d)!/(d!)^5
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation uses a prime sieve to compute the multinomial
 *  coefficients by prime exponents (Legendre's formula).  Intermediate
 *  sums are accumulated in two unsigned accumulators to avoid
 *  underflow on unsigned seq_t types. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Global cache for primes                                           */
/* ------------------------------------------------------------------ */
static uint32_t *prime_list  = NULL;   /* primes up to prime_limit  */
static size_t   prime_count  = 0;      /* number of primes          */
static uint64_t prime_limit  = 0;      /* highest prime computed    */

/* ------------------------------------------------------------------ */
/*  Ensure that primes up to 'limit' are generated.                  */
/* ------------------------------------------------------------------ */
static void ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit) return;

    /* allocate a sieve array */
    uint8_t *mark = (uint8_t *)calloc(limit + 1, 1);
    if (!mark) { /* fatal */ return; }

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (!mark[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                mark[j] = 1;

    /* count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!mark[i]) ++cnt;

    /* reallocate prime list */
    uint32_t *new_list = (uint32_t *)realloc(prime_list, cnt * sizeof(uint32_t));
    if (!new_list) { free(mark); return; }
    prime_list  = new_list;
    prime_count = cnt;

    size_t pos = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!mark[i]) prime_list[pos++] = (uint32_t)i;

    free(mark);
    prime_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  Legendre exponent of prime p in n!                                */
/* ------------------------------------------------------------------ */
static inline uint64_t legendre(uint64_t n, uint32_t p)
{
    uint64_t exp = 0;
    uint64_t pp  = p;
    while (pp <= n) {
        exp += n / pp;
        if (pp > n / p) break;
        pp *= p;
    }
    return exp;
}

/* ------------------------------------------------------------------ */
/*  Cache for multinomial values M[d] = (5d)!/(d!)^5                 */
/* ------------------------------------------------------------------ */
static seq_t  *M_val        = NULL;
static bool   *M_computed   = NULL;
static size_t   M_size      = 0;

/* ------------------------------------------------------------------ */
/*  Ensure cache arrays large enough for index 'n'.                    */
/* ------------------------------------------------------------------ */
static void ensure_M_size(size_t n)
{
    if (n < M_size) return;
    size_t new_size = n + 1;

    seq_t *newM      = (seq_t *)realloc(M_val, new_size * sizeof(seq_t));
    bool  *newC      = (bool  *)realloc(M_computed, new_size * sizeof(bool));

    if (!newM || !newC) return;

    for (size_t i = M_size; i < new_size; ++i) {
        newM[i] = 0;
        newC[i] = false;
    }

    M_val      = newM;
    M_computed = newC;
    M_size     = new_size;
}

/* ------------------------------------------------------------------ */
/*  Compute multinomial (5d)!/(d!)^5 using prime exponents            */
/* ------------------------------------------------------------------ */
static seq_t compute_M_value(uint64_t d)
{
    seq_t res = 1;
    uint64_t m = 5 * d;

    for (size_t idx = 0; idx < prime_count && (uint64_t)prime_list[idx] <= m; ++idx) {
        uint32_t p  = prime_list[idx];
        uint64_t em = legendre(m, p);
        uint64_t ed = legendre(d, p);

        /* exponent of p in multinomial */
        long long e = (long long)em - 5LL * (long long)ed;
        if (e <= 0) continue;

        /* repeated squaring for base^e */
        seq_t base = (seq_t)p;
        seq_t pow  = 1;
        uint64_t ee = (uint64_t)e;
        while (ee) {
            if (ee & 1) pow *= base;
            base *= base;
            ee >>= 1;
        }
        res *= pow;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/*  Retrieve cached M[d] or compute if necessary.                     */
/* ------------------------------------------------------------------ */
static seq_t get_M(uint64_t d)
{
    ensure_M_size(d);
    if (!M_computed[d]) {
        M_val[d] = compute_M_value(d);
        M_computed[d] = true;
    }
    return M_val[d];
}

/* ------------------------------------------------------------------ */
/*  Möbius function via prime factorisation.                          */
/* ------------------------------------------------------------------ */
static int mu_of(uint64_t n)
{
    if (n == 1) return 1;
    int cnt = 0;
    for (size_t idx = 0; idx < prime_count &&
                        (uint64_t)prime_list[idx] * prime_list[idx] <= n; ++idx) {
        uint32_t p = prime_list[idx];
        int exp = 0;
        while (n % p == 0) {
            n /= p;
            exp++;
            if (exp > 1) return 0;    /* squared prime factor */
        }
        if (exp == 1) cnt++;
    }
    if (n > 1) cnt++;                  /* remaining prime factor */
    return (cnt % 2 == 0) ? 1 : -1;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;

    /* ensure primes up to 5N for multinomial computation */
    ensure_primes(5 * N);

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (uint64_t d = 1; d * d <= N; ++d) {
        if (N % d != 0) continue;

        uint64_t d1 = d;
        uint64_t d2 = N / d;

        /* term for d1 */
        int mu1 = mu_of(d2);
        if (mu1 != 0) {
            seq_t val = get_M(d1);
            if (mu1 == 1) sum_pos += val;
            else          sum_neg += val;
        }

        if (d1 == d2) continue;   /* avoid double counting perfect square */

        /* term for d2 */
        int mu2 = mu_of(d1);
        if (mu2 != 0) {
            seq_t val = get_M(d2);
            if (mu2 == 1) sum_pos += val;
            else          sum_neg += val;
        }
    }

    /* sum_pos >= sum_neg by construction */
    seq_t result = (sum_pos - sum_neg) / (seq_t)N;
    return result;
}
