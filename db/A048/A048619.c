/*
 * A048619  a(n) = LCM(binomial(n,0), ..., binomial(n,n)) /
 *          binomial(n,floor(n/2)).
 * https://oeis.org/A048619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(1..n+1)/(ceil((n+1)/2)*binomial(n+1,floor((n+1)/2)))
 * Range:     u8 n <= 21, u16 n <= 41, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* computed by prime‐factor exponent cancellation */

/* ---------- prime sieve helpers ---------- */
static idx_t *prime_list = NULL;
static size_t   prime_cnt  = 0;
static idx_t    prime_lim  = 0;

static void build_primes(idx_t limit, idx_t **out, size_t *cnt)
{
    size_t sz = (size_t)(limit + 1);
    uint8_t *is_prime = (uint8_t *)malloc(sz);
    if (!is_prime) { *out = NULL; *cnt = 0; return; }
    memset(is_prime, 1, sz);
    if (sz > 0) is_prime[0] = 0;
    if (sz > 1) is_prime[1] = 0;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= limit; q += p)
                is_prime[q] = 0;
        }
    }
    size_t c = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++c;
    *cnt = c;
    *out = (idx_t *)malloc(c * sizeof(idx_t));
    if (!*out) { free(is_prime); *cnt = 0; return; }
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) (*out)[idx++] = i;
    free(is_prime);
}

static void ensure_primes(idx_t limit)
{
    if (prime_lim >= limit) return;
    free(prime_list);
    build_primes(limit, &prime_list, &prime_cnt);
    prime_lim = limit;
}

/* ---------- factorial prime exponent helper ---------- */
static idx_t vp_factorial(idx_t n, idx_t p)
{
    idx_t cnt = 0;
    while (n) {
        n /= p;
        cnt += n;
    }
    return cnt;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    idx_t m = n + 1;                     /* m = n+1   */
    idx_t k = m / 2;                     /* floor(m/2) */
    idx_t ceil_half = (m + 1) / 2;        /* ceil(m/2)  */

    ensure_primes(m);

    seq_t result = 1;

    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        idx_t p = prime_list[idx];
        if (p > m) break;

        /* exponent of p in lcm(1..m) : floor(log_p(m)) */
        idx_t eL = 0;
        idx_t power = p;
        while (power <= m) {
            ++eL;
            if (power > m / p) break;
            power *= p;
        }

        /* exponent of p in ceil_half */
        idx_t eDen_ceil = 0;
        idx_t x = ceil_half;
        while (x % p == 0) {
            ++eDen_ceil;
            x /= p;
        }

        /* exponent of p in binom(m,k) via Legendre's formula */
        idx_t eDen_binom = vp_factorial(m, p)
                         - vp_factorial(k,   p)
                         - vp_factorial(m - k, p);

        idx_t eDen = eDen_ceil + eDen_binom;
        int diff = (int)(eL - eDen);    /* should be >= 0 */

        while (diff > 0) {
            result = result * (seq_t)p;
            --diff;
        }
    }

    return result;
}
