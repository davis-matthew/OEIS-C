/*
 * A161510  Number of primes formed as the sum of distinct divisors of n,
 *          counted with repetition.
 * https://oeis.org/A161510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of the divisors of n whose sum is prime
 * Range:     u8 n <= 59, u16 n <= 239, u32 n <= 1259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses trial‑division factorisation and a simple
   primality test for the subset sums.  It works for all n ≥ 1 and is
   fast enough for the typical limits of this sequence. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The types seq_t and idx_t are defined by the build.  We only use
   the operations allowed in the requirements.  Elementary
   functions that are not provided by the utils are re‑implemented
   locally. */

/* ------------------------------------------------------------------ */
/*  Helper: trial‑division factorisation of a 64‑bit integer.          */
/*  On entry bits of input[0] are zeroed; the result is placed in
    `primes` and `exps`.  The number of distinct prime factors is
    returned in *cnt.                                                  */
static void factor_u64(uint64_t n,
                       uint64_t *primes,
                       uint64_t *exps,
                       size_t *cnt)
{
    size_t c = 0;
    uint64_t d = 2;
    while (d * d <= n) {
        if (n % d == 0) {
            uint64_t e = 0;
            while (n % d == 0) {
                n /= d;
                ++e;
            }
            primes[c] = d;
            exps[c] = e;
            ++c;
        }
        d = (d == 2) ? 3 : d + 2;  /* skip even numbers after 2 */
    }
    if (n > 1) {
        primes[c] = n;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* ------------------------------------------------------------------ */
/*  Helper: deterministic Miller–Rabin for 64‑bit integers.  Returns
    true if x is definitely prime, false otherwise.  The bases used
    are the standard deterministic set for 64‑bit numbers.           */
static uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    __uint128_t res = ( __uint128_t ) a * b;
    return (uint64_t)(res % mod);
}

static uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    while (exp) {
        if (exp & 1) result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_big(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
        if (n % small[i] == 0) return n == small[i];
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Deterministic bases for 64‑bit */
    static const uint64_t bases[] = {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Simple primality test for small integers (used for subset sums)
    where the sum is at most the sum of the divisors of n.            */
static bool is_prime_small(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                             */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for offset 1 */

    /* ---- 1. Compute all divisors of n ---- */
    uint64_t nn = (uint64_t)n;
    uint64_t prime_f[64], exp_f[64];
    size_t fp_cnt = 0;
    factor_u64(nn, prime_f, exp_f, &fp_cnt);

    /* Generate list of divisors */
    size_t max_divs = 1;
    for (size_t i = 0; i < fp_cnt; ++i)
        max_divs *= (size_t)(exp_f[i] + 1);

    /* Allocate divisor array dynamically */
    uint64_t *divs = (uint64_t *)calloc(max_divs, sizeof(uint64_t));
    if (!divs) return 0;
    divs[0] = 1;
    size_t cur_len = 1;

    for (size_t i = 0; i < fp_cnt; ++i) {
        uint64_t p = prime_f[i];
        size_t e = exp_f[i];
        size_t base_len = cur_len;
        uint64_t pow = 1;
        for (size_t k = 1; k <= e; ++k) {
            pow *= p;
            for (size_t j = 0; j < base_len; ++j) {
                divs[cur_len++] = divs[j] * pow;
            }
        }
    }

    /* ---- 2. Sum of all divisors (maximum subset sum) ---- */
    seq_t sumDivs = 0;
    for (size_t i = 0; i < cur_len; ++i)
        sumDivs += divs[i];
    idx_t max_sum = (idx_t)sumDivs;  /* safe cast; sums are small for n ≤ 10000 */

    /* Allocate DP array for subset sums */
    seq_t *dp = (seq_t *)calloc((size_t)max_sum + 1, sizeof(seq_t));
    if (!dp) { free(divs); return 0; }
    dp[0] = 1;    /* empty subset contributes sum 0 */

    /* ---- 3. 0/1 knapsack DP over all divisors ---- */
    for (size_t i = 0; i < cur_len; ++i) {
        idx_t d = (idx_t)divs[i];
        for (idx_t s = max_sum; s >= d; --s)
            dp[s] += dp[s - d];
    }

    /* ---- 4.  Count all subset sums that are prime ---- */
    seq_t ans = 0;
    for (idx_t s = 2; s <= max_sum; ++s) {
        if (is_prime_small((uint64_t)s))
            ans += dp[s];
    }

    /* Clean up */
    free(dp);
    free(divs);

    return ans;
}
