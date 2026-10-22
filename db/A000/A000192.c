/*
 * A000192  Generalized Euler numbers c(6,n).
 * https://oeis.org/A000192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sum_{k=0..n} binomial(2n,2k) * c(k) * a(n-k) = 2*(-9)^n, where
 *            c(0) = 1 and c(k) = 2*(-16)^k for k >= 1 (from the e.g.f.
 *            2*cos(3x)/(2*cos(4x)-1), i.e. f(x)*(2*cos(4x)-1) = 2*cos(3x));
 *            so a(n) = 2*(-9)^n - Sum_{k=1..n}
 *            binomial(2n,2k)*2*(-16)^k*a(n-k), a(0) = 2. Positive and
 *            negative parts are sum...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A192_MAXN 20001

static seq_t a192_memo[A192_MAXN];
static idx_t a192_have = 0;

static void a192_step(void)
{
    idx_t m = a192_have;
    seq_t pos = 0, neg = 0;
    seq_t p9 = 1;
    idx_t i, k;

    for (i = 0; i < m; i++)
        p9 *= 9;
    if (m % 2 == 0)
        pos += p9 * 2;
    else
        neg += p9 * 2;

    {
        seq_t c = 1;   /* binomial(2m, 2k) */
        seq_t p16 = 1; /* 16^k */
        for (k = 1; k <= m; k++) {
            seq_t t;
            c = c * (seq_t)(2 * m - 2 * k + 2) / (seq_t)(2 * k - 1);
            c = c * (seq_t)(2 * m - 2 * k + 1) / (seq_t)(2 * k);
            p16 *= 16;
            t = c * p16;
            t *= a192_memo[m - k];
            t *= 2;
            if (k % 2 == 1)
                pos += t;
            else
                neg += t;
        }
    }

    a192_memo[m] = pos - neg;
    a192_have++;
}

seq_t a(idx_t n)
{
    if (n >= A192_MAXN)
        return 0;
    while (a192_have <= n)
        a192_step();
    return a192_memo[n];
}
