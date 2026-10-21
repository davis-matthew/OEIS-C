/*
 * A089278  Coefficient triangle for computation of column numbers of triangle
 *          A071951 (Legendre-Stirling).
 * https://oeis.org/A089278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n, m) = A089500(n) * ((-1)^(m+n)) * (2*m+1) * (m*(m+1))^n /
 *            ((m+n+1)! * (n-m)!) with row offset 1
 * Range:     i8 n <= 7, i16 n <= 17, i32 n <= 26, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* ---------- Utility: unsigned integer sqrt for up to 64‑bit values ---------- */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (1ULL << 32);      /* hi^2 > 2^64 >= n           */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* ---------- Prime infrastructure (up to 20022) --------------------------- */
#define MAX_PRIME_VAL 20022
static int primes_arr[MAX_PRIME_VAL + 1];
static int prime_idx[MAX_PRIME_VAL + 1];
static int prime_count = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    primes_initialized = true;
    memset(prime_idx, -1, sizeof(prime_idx));
    bool comp[MAX_PRIME_VAL + 1] = {0};
    for (int i = 2; i <= MAX_PRIME_VAL; ++i) {
        if (!comp[i]) {
            prime_idx[i] = prime_count;
            primes_arr[prime_count++] = i;
            if ((long long)i * i <= MAX_PRIME_VAL) {
                for (int j = i * i; j <= MAX_PRIME_VAL; j += i) comp[j] = 1;
            }
        }
    }
}

/* ---------- Factorial exponent in n! for a given prime  ------------------- */
static inline int exp_in_factorial(int p, idx_t n)
{
    int e = 0;
    while (n) { n /= p; e += (int)n; }
    return e;
}

/* ---------- Factorisation of an integer <= MAX_PRIME_VAL ----------------- */
static void factor_small(int x, int exp_arr[])
{
    memset(exp_arr, 0, prime_count * sizeof(int));
    int tmp = x;
    for (int i = 0; i < prime_count && primes_arr[i] * primes_arr[i] <= tmp; ++i) {
        int p = primes_arr[i];
        if (tmp % p == 0) {
            int cnt = 0;
            while (tmp % p == 0) { tmp /= p; ++cnt; }
            exp_arr[i] = cnt;
        }
    }
    if (tmp > 1) { /* tmp is prime */
        int idx = prime_idx[tmp];
        exp_arr[idx] = 1;
    }
}

/* ---------- Main sequence function -------------------------------------- */
seq_t a(idx_t n)
{
    init_primes();
    if (n == 0) return (seq_t)0;     /* undefined for offset 1, but defensive */

    /* Find row r such that r*(r+1)/2 >= n */
    idx_t t = 8ULL * n - 7ULL;
    idx_t sq = floor_sqrt_u64(t);
    idx_t r = (sq - 1) / 2 + 1;
    idx_t prev = (r - 1) * r / 2;
    idx_t m = n - prev; /* 1‑based column within the row */

    /* Arrays for prime exponents (size PRIME_COUNT) */
    int lcm_exp[MAX_PRIME_VAL + 1];
    int num_exp_tmp[MAX_PRIME_VAL + 1];
    int den_exp_tmp[MAX_PRIME_VAL + 1];
    int factor_exp[MAX_PRIME_VAL + 1];
    int final_exp[MAX_PRIME_VAL + 1];

    memset(lcm_exp, 0, prime_count * sizeof(int));

    /* ---- Compute lcm of reduced denominators for this row (A089500(r)) ---- */
    for (int k = 1; k <= r; ++k) {
        /* Denominator exponents for this k */
        memset(den_exp_tmp, 0, prime_count * sizeof(int));
        for (int pi = 0; pi < prime_count; ++pi) {
            int p = primes_arr[pi];
            den_exp_tmp[pi] = exp_in_factorial(p, k + r + 1) + exp_in_factorial(p, r - k);
        }

        /* Numerator exponents for this k */
        memset(num_exp_tmp, 0, prime_count * sizeof(int));
        int base_k = k * (k + 1);
        factor_small(base_k, factor_exp);
        for (int pi = 0; pi < prime_count; ++pi) num_exp_tmp[pi] += factor_exp[pi] * r;
        factor_small(2 * k + 1, factor_exp);
        for (int pi = 0; pi < prime_count; ++pi) num_exp_tmp[pi] += factor_exp[pi];

        /* Reduced denominator exponents: den - min(den,num) */
        for (int pi = 0; pi < prime_count; ++pi) {
            int ed = den_exp_tmp[pi];
            int en = num_exp_tmp[pi];
            int er = ed - (ed < en ? ed : en);   /* max(0, ed - en) */
            if (er > lcm_exp[pi]) lcm_exp[pi] = er;
        }
    }

    /* ---- Compute denominator exponents for the target (m) ---- */
    memset(den_exp_tmp, 0, prime_count * sizeof(int));
    for (int pi = 0; pi < prime_count; ++pi) {
        int p = primes_arr[pi];
        den_exp_tmp[pi] = exp_in_factorial(p, m + r + 1) + exp_in_factorial(p, r - m);
    }

    /* ---- Compute numerator exponents for the target (m) ---- */
    memset(num_exp_tmp, 0, prime_count * sizeof(int));
    int base = m * (m + 1);
    factor_small(base, factor_exp);
    for (int pi = 0; pi < prime_count; ++pi) num_exp_tmp[pi] += factor_exp[pi] * r;
    factor_small(2 * m + 1, factor_exp);
    for (int pi = 0; pi < prime_count; ++pi) num_exp_tmp[pi] += factor_exp[pi];

    /* ---- Final prime exponents: lcm + num - den ---- */
    for (int pi = 0; pi < prime_count; ++pi)
        final_exp[pi] = lcm_exp[pi] + num_exp_tmp[pi] - den_exp_tmp[pi];

    /* ---- Build integer value from prime exponents ---- */
    __int128 val = 1;
    for (int pi = 0; pi < prime_count; ++pi) {
        int p = primes_arr[pi];
        int e = final_exp[pi];
        for (int j = 0; j < e; ++j) val *= p;
    }

    /* Apply sign */
    if (((r + m) & 1ULL) != 0ULL) val = -val;

    return (seq_t)val;
}
