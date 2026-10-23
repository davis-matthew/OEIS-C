/*
 * A174102  Triangle read by rows: T(n, m) = floor(binomial(n+1, m)*
 *          binomial(n+2, m)/(2*m+2)), 1 <= m <= n.
 * https://oeis.org/A174102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor( binomial(r+1, m)*binomial(r+2, m)/(2*m+2) ) where
 *            (r,m) is obtained from the flattened index n such that 1 + 2 + …
 *            + (r-1) < n ≤ 1 + 2 + … + r
 * Range:     u8 n <= 23, u16 n <= 58, u32 n <= 198, u64 n <= 681,
 *            u128 n <= 2380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime utilities ---------- */
#define PRIMELIMIT 20000
static int primes[PRIMELIMIT + 1];
static int prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt != 0)
        return;                 /* already initialized */
    bool *is_prime = (bool *)malloc((PRIMELIMIT + 1) * sizeof(bool));
    if (!is_prime) abort();
    for (int i = 2; i <= PRIMELIMIT; ++i)
        is_prime[i] = true;
    for (int i = 2; i * i <= PRIMELIMIT; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= PRIMELIMIT; j += i)
                is_prime[j] = false;
    for (int i = 2; i <= PRIMELIMIT; ++i)
        if (is_prime[i])
            primes[prime_cnt++] = i;
    free(is_prime);
}

static int exponent_in_factorial(idx_t n, int p)
{
    int exp = 0;
    idx_t div = p;
    while (div <= n) {
        exp += n / div;
        if (div > n / p) /* prevents overflow of div * p */
            break;
        div *= p;
    }
    return exp;
}

/* ---------- Core triangular term computation ---------- */
static void process_value(int val, int *cnt, seq_t *res)
{
    int t = val;
    /* trial division by all primes up to sqrt(t) */
    for (int i = 0; i < prime_cnt && (int)primes[i] * primes[i] <= t; ++i) {
        int p = primes[i];
        while (t % p == 0) {
            if (cnt[i] > 0)
                cnt[i]--;
            else
                *res *= p;
            t /= p;
        }
    }
    if (t > 1) {
        /* t is now a prime (maybe in our list) */
        int idx = -1;
        for (int i = 0; i < prime_cnt; ++i) {
            if (primes[i] == t) {
                idx = i;
                break;
            }
        }
        if (idx != -1) {
            if (cnt[idx] > 0)
                cnt[idx]--;
            else
                *res *= t;
        } else {
            *res *= t;
        }
    }
}

/* Compute T(r,m) for given triangle coordinates */
static seq_t compute_T(idx_t n, idx_t m)
{
    init_primes();

    int *cnt = (int *)malloc(prime_cnt * sizeof(int));
    if (!cnt)
        abort();
    for (int i = 0; i < prime_cnt; ++i)
        cnt[i] = 0;

    /* Count prime exponents in denominator:
       Denominator = (m!)^2 * 2 * (m+1) */
    for (int i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        int e = 2 * exponent_in_factorial(m, p); /* from (m!)^2 */
        int tmp = m + 1;
        while (tmp % p == 0) {
            e++;
            tmp /= p;
        }
        if (p == 2)        /* extra factor from 2*(m+1) */
            e++;
        cnt[i] = e;
    }

    seq_t res = 1;

    /* Multiplicative construction of the two binomial numerators   */
    for (idx_t i = 1; i <= m; ++i) {
        int val = (int)(n + 1 - (i - 1));
        process_value(val, cnt, &res);
    }
    for (idx_t i = 1; i <= m; ++i) {
        int val = (int)(n + 2 - (i - 1));
        process_value(val, cnt, &res);
    }

    /* Divide remaining denominator primes (floor division) */
    for (int i = 0; i < prime_cnt; ++i) {
        while (cnt[i] > 0) {
            res /= (seq_t)primes[i];
            cnt[i]--;
        }
    }

    free(cnt);
    return res;
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    /* Find the row r such that sum_{k=1}^r k >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column m (1‑based) within row r */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t m = n - (idx_t)prev;   /* 1 ≤ m ≤ r */

    return compute_T(r, m);
}
