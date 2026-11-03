/*
 * A063648  Smallest c such that 1/n=1/c+1/b has integer solutions with c>b.
 * https://oeis.org/A063648
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A063648: smallest c such that 1/n = 1/b + 1/c has integer solutions with c > b.
   Formula: let k be the largest divisor of n² that is < n.
            Then a(n) = n + n² / k.
*/

#include <stddef.h>
#include <stdint.h>

/* recursive helper: find the largest divisor of n² that is < n */
static void dfs_max_div(size_t idx, seq_t cur, const idx_t *pr, const int *exp,
                        size_t npr, const seq_t n, seq_t *best)
{
    if (idx == npr) {
        if (cur < n && cur > *best)
            *best = cur;
        return;
    }

    seq_t val = cur;
    int maxe = exp[idx] * 2;              /* exponent of this prime in n² */
    for (int e = 0; e <= maxe; ++e) {
        if (val >= n)              /* further exponents only enlarge val */
            break;
        dfs_max_div(idx + 1, val, pr, exp, npr, n, best);
        val *= pr[idx];             /* next exponent for the same prime */
    }
}

seq_t a(idx_t n)
{
    if (n < 2)                     /* below the defined offset */
        return 0;

    /* factorise n */
    idx_t primes[16];
    int    exps[16];
    size_t cnt = 0;

    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        primes[cnt] = tmp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* find the largest divisor of n² that is < n */
    seq_t kmax = 1;                 /* at worst k = 1 */
    dfs_max_div(0, (seq_t)1, primes, exps, cnt, (seq_t)n, &kmax);

    /* compute a(n) = n + n² / kmax */
    uint64_t n64 = (uint64_t)n;
    uint64_t res64 = n64 + (n64 * n64) / (uint64_t)kmax;
    return (seq_t)res64;
}
