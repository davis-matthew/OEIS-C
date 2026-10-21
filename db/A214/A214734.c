/*
 * A214734  Sum_{k=1..n} floor(k*p/q), where (p,q) are either coprime positive
 *          integers or q=1 or p=1, n*p>=q, ordered by (n + p + q) ascending,
 *          then n ascending, then p ascending.
 * https://oeis.org/A214734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(k*p/q) The sequence is obtained by
 *            listing all triples (n,p,q) with n*p >= q and (p==1 || q==1 ||
 *            gcd(p,q)==1) in the order of increasing n+p+q, then increasing
 *            n, then increasing p. For each such triple the value above is
 *            computed and placed in the list
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdint.h>   /* uint64_t etc. */
#include "../utils/gcd.h"

/* compute Sum_{k=1..n} floor(k*p/q)  */
static seq_t sum_floor(idx_t n, idx_t p, idx_t q)
{
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k)
        total += (((seq_t)k * (seq_t)p) / (seq_t)q);
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined, but harmless */

    idx_t count = 0;
    /* iterate over planes s = n+p+q ascending */
    for (idx_t s = 3; ; ++s) {
        for (idx_t ni = 1; ni <= s-2 && count < n; ++ni) {
            for (idx_t p = 1; p <= s-ni-1 && count < n; ++p) {
                idx_t q = s - ni - p;
                if (ni * p < q)                 /* n*p >= q needed */
                    continue;
                if (p > 1 && q > 1 && gcd((seq_t)p,(seq_t)q)!=1)
                    continue;                  /* coprime unless p==1 or q==1 */
                seq_t val = sum_floor(ni, p, q);
                ++count;
                if (count == n)
                    return val;
            }
        }
    }
    /* unreachable */
    return 0;
}
