/*
 * A200473  Irregular triangle read by rows: T(n,k) = number of ways to assign
 *          n people to d_k unlabeled groups of equal size (where d_k is the
 *          k-th divisor of n).
 * https://oeis.org/A200473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by flattening the triangle T(n,k) = n! / (d_k!)
 *            / ( (n/d_k)!^d_k ) (d_k = k‑th divisor of n) over all rows n>=1
 *            and all divisors d_k of n
 * Range:     u8 n <= 21, u16 n <= 39, u32 n <= 68, u64 n <= 116,
 *            u128 n <= 214, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* The build system provides seq_t and idx_t. */

/* ----------  global tables  ---------- */
static idx_t *spf = NULL;          /* smallest prime factor table */
static size_t spf_len = 0;         /* size of spf array = limit+1 */

static idx_t *primes = NULL;       /* primes up to current limit */
static size_t prime_cnt = 0;

static idx_t *cum_tau = NULL;      /* cum_tau[i] = Σ_{j=1}^{i+1} τ(j) */
static size_t cum_len = 0;

/* ----------  build spf and primes  ---------- */
static void build_spf_and_primes(idx_t limit)
{
    free(spf);
    free(primes);
    spf = (idx_t *)calloc((size_t)limit + 1, sizeof(idx_t));
    if (!spf) exit(EXIT_FAILURE);
    spf[0] = 0;
    if (limit >= 1)
        spf[1] = 1;

    /* sieve for smallest prime factors */
    for (idx_t i = 2; i * i <= limit; ++i)
        if (spf[i] == 0) {
            spf[i] = i;
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0)
                    spf[j] = i;
        }
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == 0)
            spf[i] = i;

    spf_len = (size_t)limit + 1;

    primes = (idx_t *)calloc(spf_len, sizeof(idx_t));
    if (!primes) exit(EXIT_FAILURE);
    prime_cnt = 0;
    for (idx_t i = 2; i < spf_len; ++i)
        if (spf[i] == i)
            primes[prime_cnt++] = i;
}

static void ensure_spf(idx_t limit)
{
    if (limit >= (idx_t)spf_len)
        build_spf_and_primes(limit);
}

/* ----------  divisor count τ  ---------- */
static size_t tau(idx_t n)
{
    if (n <= 1) return 1;
    size_t res = 1;
    idx_t tmp = n;
    while (tmp > 1) {
        idx_t p = spf[tmp];
        size_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        res *= (e + 1);
    }
    return res;
}

/* ----------  generate all divisors of n  ---------- */
static void gen_divs_rec(const idx_t *pr, const idx_t *exp,
                         size_t pcnt, size_t idx,
                         idx_t cur, idx_t *out, size_t *cnt)
{
    if (idx == pcnt) {
        out[(*cnt)++] = cur;
        return;
    }
    idx_t p = pr[idx];
    idx_t emax = exp[idx];
    idx_t val = cur;
    for (idx_t e = 0; e <= emax; ++e) {
        gen_divs_rec(pr, exp, pcnt, idx + 1, val, out, cnt);
        val *= p;
    }
}

static void divisors_of(idx_t n, idx_t *out, size_t *cnt)
{
    idx_t fac_pr[64], fac_exp[64];
    size_t pcnt = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        idx_t p = spf[tmp];
        size_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        fac_pr[pcnt] = p;
        fac_exp[pcnt] = e;
        ++pcnt;
    }
    *cnt = 0;
    gen_divs_rec(fac_pr, fac_exp, pcnt, 0, (idx_t)1, out, cnt);
    /* small insertion sort (τ(n) is small) */
    for (size_t i = 1; i < *cnt; ++i)
        for (size_t j = i; j > 0 && out[j-1] > out[j]; --j) {
            idx_t tmp = out[j];
            out[j] = out[j-1];
            out[j-1] = tmp;
        }
}

/* ----------  exponent of prime in factorial  ---------- */
static idx_t exp_fact(idx_t N, idx_t p)
{
    idx_t e = 0;
    for (idx_t d = N / p; d; d /= p)
        e += d;
    return e;
}

/* ----------  T(n,d) computation  ---------- */
static seq_t compute_T(idx_t n, idx_t d)
{
    idx_t m = n / d;
    ensure_spf(n);                   /* primes up to n are needed */
    seq_t res = (seq_t)1;
    for (size_t i = 0; i < prime_cnt && primes[i] <= n; ++i) {
        idx_t p = primes[i];
        idx_t e = exp_fact(n, p) - exp_fact(d, p) - d * exp_fact(m, p);
        for (idx_t k = 0; k < e; ++k)
            res *= (seq_t)p;
    }
    return res;
}

/* ----------  extend cumulative τ table until >=needed  ---------- */
static void extend_cumulative(idx_t needed)
{
    while (cum_len == 0 || cum_tau[cum_len-1] < needed) {
        idx_t row = (idx_t)(cum_len + 1);   /* new row number (1‑based) */
        ensure_spf(row);
        size_t tt = tau(row);
        idx_t new_sum = (cum_len == 0) ? tt : cum_tau[cum_len-1] + tt;
        idx_t *tmp = (idx_t *)realloc(cum_tau, (cum_len + 1) * sizeof(idx_t));
        if (!tmp) exit(EXIT_FAILURE);
        cum_tau = tmp;
        cum_tau[cum_len++] = new_sum;
    }
}

/* ----------  find row and zero‑based position in that row  ---------- */
static void row_and_pos(idx_t n, idx_t *row, size_t *pos)
{
    if (n == 0) { *row = 1; *pos = 0; return; }
    extend_cumulative(n);
    size_t lo = 0, hi = cum_len;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (cum_tau[mid] >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    size_t idx = lo;                     /* index in cum_tau */
    *row = (idx_t)(idx + 1);             /* row number (1‑based) */
    idx_t prev = (idx == 0) ? 0 : cum_tau[idx-1];
    *pos = (size_t)(n - prev - 1);       /* zero‑based position */
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1 */
    idx_t row; size_t pos;
    row_and_pos(n, &row, &pos);

    /* Ensure spf data for the current row */
    ensure_spf(row);

    idx_t divs[64]; size_t cnt;
    divisors_of(row, divs, &cnt);   /* divisors sorted ascending */
    if (pos >= cnt) return (seq_t)0;  /* safety */
    idx_t d = divs[pos];
    return compute_T(row, d);
}
