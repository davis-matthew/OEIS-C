/*
 * A153494  a(n) is smallest number greater than n that, when compared
 *          divisors of a(n) with divisors of n, the d-th smallest divisor of
 *          a(n) is always <= the d-th smallest divisor of n for all values of
 *          d <= the number of divisors of n.
 * https://oeis.org/A153494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   fact_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest integer m > n such that for every d ≤ τ(n)
 * the d‑th smallest divisor of m is ≤ the d‑th smallest divisor of n.
 * The algorithm generates divisors of n and each candidate m by trial
 * division, sorts them, and compares the first τ(n) divisors.  It
 * returns the first m that satisfies the condition.  This brute‑
 * force method is sufficient for the intended range (n ≤ 10 000).
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/fact_uint64.h"

/* integer square root by binary search */
static idx_t int_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n, mid;
    while (lo <= hi) {
        mid = (lo + hi) / 2;
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* comparison function for qsort of seq_t */
static int seq_cmp(const void *a, const void *b)
{
    seq_t v1 = *(const seq_t *)a;
    seq_t v2 = *(const seq_t *)b;
    return (v1 < v2) ? -1 : (v1 > v2) ? 1 : 0;
}

/* generate sorted list of all positive divisors of n */
/* buf must have enough space (we use a static buffer of 512). */
static void gen_divisors(idx_t n, seq_t *buf, size_t *cnt_out)
{
    uint64_t n64 = (uint64_t)n;
    uint64_t primes[32];
    int exps[32];
    size_t fac_cnt;
    fact_uint64(n64, primes, exps, &fac_cnt);

    size_t cur = 1;
    buf[0] = (seq_t)1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = primes[i];
        int e = exps[i];
        size_t before = cur;
        uint64_t mult = 1;
        for (int k = 1; k <= e; ++k) {
            mult *= p;
            for (size_t j = 0; j < before; ++j) {
                buf[cur++] = (seq_t)(buf[j] * mult);
            }
        }
    }
    *cnt_out = cur;
    qsort(buf, cur, sizeof(seq_t), seq_cmp);
}

/* required function */
seq_t a(idx_t n)
{
    enum { MAX_DIVS = 512 };
    static seq_t div_n[MAX_DIVS];
    static seq_t div_cand[MAX_DIVS];
    size_t tau_n, tau_cand;

    gen_divisors(n, div_n, &tau_n);

    idx_t cand = n + 1;
    for (;;) {
        gen_divisors(cand, div_cand, &tau_cand);
        if (tau_cand < tau_n) {
            ++cand;
            continue;
        }
        bool ok = true;
        for (size_t i = 0; i < tau_n; ++i) {
            if (div_cand[i] > div_n[i]) {
                ok = false;
                break;
            }
        }
        if (ok) return (seq_t)cand;
        ++cand;
    }
}
