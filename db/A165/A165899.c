/*
 * A165899  Residue of n! after successive divisions by factors greater than
 *          n.
 * https://oeis.org/A165899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = residue of n! after successive divisions by factors > n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

/* ---------- prime sieve (max up to 100000, safe for n ≤ 10000) ---------- */
#define MAX_PRIME 115000
static uint32_t _primes[MAX_PRIME];
static size_t _prime_cnt = 0;

static void _build_prime_sieve(void)
{
    if (_prime_cnt != 0) return;                      /* already built */

    bool *is_prime = (bool*)malloc(MAX_PRIME * sizeof(bool));
    for (size_t i = 0; i < MAX_PRIME; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i < MAX_PRIME; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j < MAX_PRIME; j += i)
                is_prime[j] = false;

    for (size_t i = 2; i < MAX_PRIME; ++i)
        if (is_prime[i]) _primes[_prime_cnt++] = (uint32_t)i;

    free(is_prime);
}

/* ---------- helper: pow for seq_t (simple exponentiation by squaring) ---------- */
static seq_t _pow_seq(seq_t base, uint32_t exp)
{
    seq_t res = (seq_t)1;
    while (exp)
    {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    _build_prime_sieve();

    if (n < 2) return (seq_t)1;                       /* a(1) = 1 */

    /* keep a memoised array for prime exponents */
    static uint32_t *exp_arr = NULL;
    static size_t exp_cap   = 0;
    size_t need = (size_t)n + 1;                      /* indices 0..n */

    if (need > exp_cap)
    {
        if (exp_arr) free(exp_arr);
        exp_arr = (uint32_t*)malloc(need * sizeof(uint32_t));
        exp_cap = need;
    }
    /* initialise exponents to 0 */
    for (size_t i = 2; i <= n; ++i) exp_arr[i] = 0;

    /* Legendre's formula: compute exponent of each prime in n! */
    long double log_cur = 0.0L;
    size_t pi = 0;
    while (pi < _prime_cnt && (uint64_t)_primes[pi] <= n)
    {
        uint32_t p = _primes[pi];
        uint32_t cnt = 0, m = (uint32_t)n;
        while (m)
        {
            m /= p;
            cnt += m;
        }
        exp_arr[p] = cnt;
        log_cur += (long double)cnt * logl((long double)p);
        ++pi;
    }

    /* Iterate k = n+1, n+2, ... while current value >= k */
    for (idx_t k = n + 1; ; ++k)
    {
        long double log_k = logl((long double)k);
        /* If current < k, stop */
        if (log_cur + 1e-12L < log_k) break;

        /* factor k using primes ≤ n */
        uint64_t tmp = (uint64_t)k;
        bool can_div = true;

        /* temporary arrays for primes and their exponents in k */
        uint32_t factors[64];
        uint32_t freqs[64];
        size_t fcnt = 0;

        /* factor using primes ≤ n */
        for (size_t j = 0; j < pi && (uint64_t)_primes[j] * (uint64_t)_primes[j] <= tmp; ++j)
        {
            uint32_t p = _primes[j];
            uint32_t c = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++c;
            }
            if (c)
            {
                if (exp_arr[p] < c) { can_div = false; break; }
                factors[fcnt] = p;
                freqs[fcnt] = c;
                ++fcnt;
            }
        }
        if (!can_div) continue;

        if (tmp > 1)
        {
            /* tmp is a prime > sqrt(original k). It must be ≤ n to divide current. */
            if (tmp > (uint64_t)n || exp_arr[(uint32_t)tmp] == 0)
            {
                can_div = false;
            }
            else
            {
                if (exp_arr[(uint32_t)tmp] < 1) can_div = false;
                else
                {
                    factors[fcnt] = (uint32_t)tmp;
                    freqs[fcnt] = 1;
                    ++fcnt;
                }
            }
        }

        if (!can_div) continue;

        /* Perform division: subtract exponents and update log */
        for (size_t t = 0; t < fcnt; ++t)
        {
            uint32_t p = factors[t];
            uint32_t c = freqs[t];
            exp_arr[p] -= c;
            log_cur -= (long double)c * logl((long double)p);
        }
        /* loop continues with updated current */
    }

    /* Build final result from remaining exponents */
    seq_t result = (seq_t)1;
    for (size_t i = 2; i <= n; ++i)
    {
        uint32_t e = exp_arr[i];
        if (e == 0) continue;
        seq_t term = _pow_seq((seq_t)i, e);
        result *= term;
    }
    return result;
}
