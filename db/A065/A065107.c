/*
 * A065107  Start of the permutation of the primes in the order in which p^2
 *          first appears as a factor of a number in the Fibonacci sequence.
 * https://oeis.org/A065107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the nth prime in the ordering by the smallest k such that
 *            p² | F_k
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   gen_divisors_recursive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   This implementation computes for each prime p its first index
   where p^2 divides a Fibonacci number and then returns the primes
   sorted by these indices.  The algorithm follows the known result:

        The rank of appearance r(p) of a prime p (p≠2,5) divides
        p - (5|p), where (5|p) is the Legendre symbol.
        The smallest n with p² | F_n is
            n = r(p)          if p² | F_{r(p)}
            n = r(p) * p      otherwise.

   The code precomputes primes up to a fixed limit (300 000) which is
   sufficient for n up to about 10 000, and then applies the logic
   above for each prime.  Large‑integer operations are performed only
   with 64‑bit values; 128‑bit arithmetic is used only for the
   intermediate multiplication used in modular arithmetic.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gen_divisors_recursive.h"

/* ---------- constants ---------- */
#define PRIME_LIMIT 300000     /* sufficient to cover ~10kth term */

/* ---------- data structures ---------- */
struct PrimeInfo {
    uint32_t prime;       /* the prime */
    uint64_t first_idx;   /* smallest n with p² | F_n */
};

/* ---------- global storage ---------- */
static uint32_t *g_primes      = NULL;
static size_t   g_prime_cnt    = 0;
static struct PrimeInfo *g_info   = NULL;
static size_t   g_info_cnt    = 0;

/* ---------- modular utilities ---------- */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) res = mul_mod(res, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* ---------- Fibonacci fast doubling (mod m) ---------- */
static void fib_pair(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;
        return;
    }
    uint64_t a, b;
    fib_pair(n >> 1, mod, &a, &b);
    uint64_t two_b = mul_mod(b, 2, mod);
    uint64_t c = mul_mod(a, (two_b + mod - a) % mod, mod);    /* F_{2k} */
    uint64_t d = (mul_mod(a, a, mod) + mul_mod(b, b, mod)) % mod; /* F_{2k+1} */
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

/* ---------- prime sieve ---------- */
static void generate_primes(void)
{
    if (g_primes != NULL) return;
    bool *is_prime = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_prime) return;
    for (uint32_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;
    /* count primes */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++cnt;
    g_primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!g_primes) {free(is_prime); return;}
    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) g_primes[idx++] = i;
    g_prime_cnt = cnt;
    free(is_prime);
}

/* comparator for sorting divisors ascending */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t*)a;
    uint64_t vb = *(const uint64_t*)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* comparator for sorting prime info by first_idx, then prime */
static int cmp_prime_info(const void *a, const void *b)
{
    const struct PrimeInfo *ia = (const struct PrimeInfo*)a;
    const struct PrimeInfo *ib = (const struct PrimeInfo*)b;
    if (ia->first_idx < ib->first_idx) return -1;
    if (ia->first_idx > ib->first_idx) return 1;
    if (ia->prime < ib->prime) return -1;
    if (ia->prime > ib->prime) return 1;
    return 0;
}

/* ---------- compute first indices for all primes ---------- */
static void compute_all(void)
{
    if (g_info != NULL) return; /* already done */

    generate_primes();

    g_info = (struct PrimeInfo*)malloc(g_prime_cnt * sizeof(struct PrimeInfo));
    if (!g_info) return;
    g_info_cnt = 0;

    /* temporary structures */
    int factor_exps[32];
    uint32_t factor_primes[32];
    size_t factor_cnt;

    uint64_t *divs = NULL;
    size_t div_cap = 0, div_len = 0;

    for (size_t pi = 0; pi < g_prime_cnt; ++pi) {
        uint32_t p = g_primes[pi];
        uint64_t r; /* rank of appearance of p */

        /* special cases */
        if (p == 2) {
            r = 3;
        } else if (p == 5) {
            r = 5;
        } else {
            /* Legendre symbol (5|p) = 1 if p ≡ ±1 mod 5, = -1 if p ≡ ±2 mod 5 */
            uint64_t leg = powmod64(5, (p - 1) / 2, p);
            int e = (leg == 1) ? 1 : -1;
            uint64_t candidate = (uint64_t)p - e; /* p-1 or p+1 */

            /* factor candidate */
            uint64_t tmp = candidate;
            factor_cnt = 0;
            for (size_t ip = 0; ip < g_prime_cnt && (uint64_t)g_primes[ip] * g_primes[ip] <= tmp; ++ip) {
                uint32_t q = g_primes[ip];
                if (tmp % q == 0) {
                    factor_primes[factor_cnt] = q;
                    int cnt = 0;
                    while (tmp % q == 0) {
                        tmp /= q;
                        ++cnt;
                    }
                    factor_exps[factor_cnt] = cnt;
                    ++factor_cnt;
                }
            }
            if (tmp > 1) {
                factor_primes[factor_cnt] = (uint32_t)tmp;
                factor_exps[factor_cnt] = 1;
                ++factor_cnt;
            }

            /* generate all divisors of candidate */
            if (div_cap == 0) {
                div_cap = 64;
                divs = (uint64_t*)malloc(div_cap * sizeof(uint64_t));
            }
            div_len = 0;
            gen_divisors_recursive(factor_primes, factor_exps, factor_cnt,
                                   0, 1, divs, &div_len, &div_cap);

            /* sort divisors */
            qsort(divs, div_len, sizeof(uint64_t), cmp_uint64);

            /* find smallest divisor d with F_d ≡ 0 mod p */
            r = 0;
            for (size_t di = 0; di < div_len; ++di) {
                uint64_t dval = divs[di];
                uint64_t fd, _unused;
                fib_pair(dval, (uint64_t)p, &fd, &_unused);
                if (fd % p == 0) {
                    r = dval;
                    break;
                }
            }
            if (r == 0) r = candidate; /* should not happen */
        }

        /* Check if p² divides F_r */
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        uint64_t f_r, _unused;
        fib_pair(r, p2, &f_r, &_unused);
        uint64_t first_idx = (f_r == 0) ? r : r * (uint64_t)p;

        g_info[g_info_cnt].prime     = p;
        g_info[g_info_cnt].first_idx = first_idx;
        ++g_info_cnt;
    }

    free(divs);
    /* sort info */
    qsort(g_info, g_info_cnt, sizeof(struct PrimeInfo), cmp_prime_info);
}

/* ---------- the OEIS function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of bounds protection */
    compute_all();                  /* memoized list of primes */

    if ((idx_t)g_info_cnt < n) {
        /* n beyond precomputed range; return 0 or clamp */
        return 0;
    }
    return (seq_t)g_info[n - 1].prime;
}
