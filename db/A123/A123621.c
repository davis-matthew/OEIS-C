/*
 * A123621  Greatest k <= n/2 such that n divides binomial(n,k).
 * https://oeis.org/A123621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest k ≤ n/2 such that n divides C(n,k)
 * Range:     u8 n <= 512, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_factorial.h"

/* OEIS sequence A123621 */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;            /* offset 1 – not used but safe */

/* factor n into primes – holds at most 16 factors for n ≤ 10⁶ */
    idx_t temp = n;
    uint64_t pf[16];
    int      exp[16];
    int      fcnt = 0;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            pf[fcnt]   = p;
            exp[fcnt]  = e;
            ++fcnt;
        }
    }
    if (temp > 1) {                 /* remaining prime factor */
        pf[fcnt]   = temp;
        exp[fcnt]  = 1;
        ++fcnt;
    }

    /* search from ⌊n/2⌋ downward */
    for (idx_t k = n / 2; ; --k) {
        bool ok = true;
        for (int i = 0; i < fcnt; ++i) {
            idx_t p      = pf[i];
            int    vp_n  = exp[i];
            int    vp_k  = count_factorial(k,     p);
            int    vp_nk = count_factorial(n - k, p);
            int    vp_nf = count_factorial(n,     p);
            if (vp_nf - vp_k - vp_nk < vp_n) {
                ok = false;
                break;
            }
        }
        if (ok) return (seq_t)k;
        if (k == 0) break;          /* should never happen – safety */
    }

    return 0;                       /* fallback (should not occur) */
}
