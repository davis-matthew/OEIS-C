/*
 * A064677  a(n) = sigma(n) - D(n) - pi(n), where D(n)=A001223, pi(n)=
 *          A000720.
 * https://oeis.org/A064677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(n) - (prime(n+1) - prime(n)) - pi(n); sieve of
 *            Eratosthenes for primes, trial division for sigma
 * Range:     u8 n <= 119, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 88 terms through n = 949 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a064677_pr = NULL;
static size_t a064677_cnt = 0;
static uint64_t a064677_lim = 0;

static void a064677_build(size_t need)
{
    uint64_t lim = a064677_lim ? a064677_lim : 1024;
    while (a064677_cnt < need) {
        if (a064677_lim) lim = a064677_lim * 2;
        unsigned char *comp = (unsigned char *)malloc((size_t)lim + 1);
        if (!comp) return;
        memset(comp, 0, (size_t)lim + 1);
        size_t cnt = 0;
        uint64_t i, j;
        for (i = 2; i <= lim; i++) {
            if (!comp[i]) {
                cnt++;
                if (i * i <= lim)
                    for (j = i * i; j <= lim; j += i) comp[j] = 1;
            }
        }
        uint64_t *arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        if (!arr) { free(comp); return; }
        size_t k = 0;
        for (i = 2; i <= lim; i++)
            if (!comp[i]) arr[k++] = i;
        free(comp);
        free(a064677_pr);
        a064677_pr = arr;
        a064677_cnt = cnt;
        a064677_lim = lim;
    }
}

static seq_t a064677_sigma(uint64_t n)
{
    seq_t total = 1;
    uint64_t p;
    for (p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            seq_t term = 1, pw = 1;
            while (n % p == 0) {
                n /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            total *= term;
        }
    }
    if (n > 1) total *= ((seq_t)n + 1);
    return total;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    size_t need = (size_t)n + 1;
    if (a064677_cnt < need) a064677_build(need);
    if (a064677_cnt < need) return (seq_t)0;
    uint64_t gap = a064677_pr[n] - a064677_pr[n - 1];
    /* pi(n): number of primes <= n, binary search */
    size_t lo = 0, hi = a064677_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (a064677_pr[mid] <= (uint64_t)n) lo = mid + 1;
        else hi = mid;
    }
    seq_t s = a064677_sigma((uint64_t)n);
    return s - (seq_t)gap - (seq_t)lo;
}
