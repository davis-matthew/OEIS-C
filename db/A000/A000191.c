/*
 * A000191  Generalized tangent numbers d(3, n).
 * https://oeis.org/A000191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   from the e.g.f. A(t)*(2*cos(2t)-1) = 2*sin(t), where A(t) = Sum
 *            a(n) t^(2n+1)/(2n+1)!: a(n) = 2*(-1)^n + Sum_{k=1..n} (-1)^(k+1)
 *            * 2*4^k * binomial(2n+1,2k) * a(n-k), a(0) = 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A191_MAXN 20000

static seq_t a191_memo[A191_MAXN + 1];
static seq_t a191_row[2 * A191_MAXN + 3];
static idx_t a191_have = 0;   /* number of terms computed */
static idx_t a191_rown = 0;   /* current Pascal row index held in a191_row */
static bool a191_init = false;

seq_t a(idx_t n)
{
    if (n > A191_MAXN) return 0;
    if (!a191_init) {
        a191_row[0] = 1;
        a191_rown = 0;
        a191_init = true;
    }
    while (a191_have <= n) {
        idx_t m = a191_have;
        idx_t target = 2 * m + 1;
        while (a191_rown < target) {
            idx_t j;
            a191_rown++;
            a191_row[a191_rown] = 1;
            for (j = a191_rown - 1; j >= 1; j--)
                a191_row[j] += a191_row[j - 1];
        }
        seq_t pos = 0, neg = 0;
        if (m % 2 == 0) pos += 2; else neg += 2;
        seq_t p4 = 2;
        idx_t k;
        for (k = 1; k <= m; k++) {
            p4 *= 4;
            seq_t t = p4 * a191_row[2 * k];
            t *= a191_memo[m - k];
            if (k % 2 == 1) pos += t; else neg += t;
        }
        a191_memo[m] = pos - neg;
        a191_have++;
    }
    return a191_memo[n];
}
