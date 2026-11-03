/*
 * A057643  Least common multiple of all (k+1)'s, where the k's are the
 *          positive divisors of n.
 * https://oeis.org/A057643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm{ d+1 : d positive divisor of n }
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 95, u64 n <= 359,
 *            u128 n <= 1259, big any n
 * Validated: standard, pass, 67 terms through n = 8946 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* ---------- Trial division factorisation ----------------------------------- */
static void _factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    if (n < 2) return;
    /* factor 2 separately */
    if ((n & 1ULL) == 0) {
        pr[*cnt] = 2ULL;
        exp[*cnt] = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            exp[*cnt] += 1;
        }
        (*cnt)++;
    }
    /* odd primes */
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0) {
            pr[*cnt] = d;
            exp[*cnt] = 0;
            while (n % d == 0) {
                n /= d;
                exp[*cnt] += 1;
            }
            (*cnt)++;
        }
    }
    if (n > 1) {
        pr[*cnt] = n;
        exp[*cnt] = 1;
        (*cnt)++;
    }
}

/* Recursive generation of all divisors of n.
   For each divisor d the value (d+1) is incorporated into the running LCM. */
static void
dfs_divs(const uint64_t *pr,
         const uint64_t *exp,
         size_t       cnt,
         size_t       idx,
         uint64_t     cur,
         seq_t       *lcm_out)
{
    if (idx == cnt) {                     /* cur is a divisor of n            */
        seq_t val = (seq_t)(cur + 1);      /* d + 1                            */
        seq_t g   = gcd(*lcm_out, val);
        *lcm_out = (*lcm_out / g) * val;   /* lcm = a/g * b                     */
        return;
    }

    uint64_t p    = pr[idx];
    uint64_t mult = 1;                    /* p^e for current exponent e       */
    for (uint64_t e = 0; e <= exp[idx]; ++e) {
        dfs_divs(pr, exp, cnt, idx + 1, cur * mult, lcm_out);
        mult *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n = 0, harmless  */

    /* Factorise n */
    uint64_t pr[64], exp[64];
    size_t cnt = 0;
    _factor_u64((uint64_t)n, pr, exp, &cnt);

    /* Compute LCM of (d+1) over all divisors d of n. */
    seq_t lcm = (seq_t)1;
    dfs_divs(pr, exp, cnt, 0, 1, &lcm);

    return lcm;
}
