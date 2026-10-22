/*
 * A006350  Related to series-parallel networks.
 * https://oeis.org/A006350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n-1} A000084(k)*A006349(n-k), n >= 2, a(1) = 0,
 *            where A006349(m) = 2*c(m) - 1, c(m) = Sum_{d|m} d*s(d), s(1) =
 *            1, s(d) = A000084(d)/2 for d >= 2, and A000084 = Euler transform
 *            of s: m*A000084(m) = c(m) + Sum_{k=1..m-1} c(k)*A000084(m-k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *Aa = NULL;   /* A000084 */
static seq_t *Cc = NULL;   /* c(m) = Sum_{d|m} d*s(d) */
static idx_t len = 0;      /* entries 0..len-1 computed */
static idx_t cap = 0;

static void extend(idx_t n) {
    if (n < len) return;
    if (n + 1 > cap) {
        idx_t newcap = cap * 2;
        if (newcap < n + 1) newcap = n + 1;
        if (newcap < 64) newcap = 64;
        Aa = grow_zero(Aa, cap, newcap);
        Cc = grow_zero(Cc, cap, newcap);
        cap = newcap;
    }
    idx_t m = len;
    if (m == 0) {
        Aa[0] = 0;
        Cc[0] = 0;
        m = 1;
    }
    for (; m <= n; m++) {
        if (m == 1) {
            Aa[1] = 1;
            Cc[1] = 1;
            continue;
        }
        /* contribution of proper divisors to c(m) */
        seq_t cp = 0;
        for (idx_t d = 1; d <= m / 2; d++) {
            if (m % d == 0) {
                if (d == 1) cp += 1;
                else cp += (seq_t)d * (Aa[d] / 2);
            }
        }
        seq_t t = cp;
        for (idx_t k = 1; k < m; k++)
            t += Cc[k] * Aa[m - k];
        /* A(m) = 2*t/m, avoiding computing 2*t */
        if (m % 2 == 0) Aa[m] = t / (seq_t)(m / 2);
        else Aa[m] = t / (seq_t)m * 2;
        Cc[m] = cp + (seq_t)m * (Aa[m] / 2);
    }
    len = n + 1;
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    extend(n - 1);
    seq_t result = 0;
    for (idx_t k = 1; k < n; k++)
        result += Aa[k] * (2 * Cc[n - k] - 1);
    return result;
}
