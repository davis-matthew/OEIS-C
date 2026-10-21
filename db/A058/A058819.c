/*
 * A058819  a(0) = 1, a(1) = 4; for n >= 2 a(n) is the number of degree-n
 *          monic reducible polynomials over GF(4), i.e., a(n) = 4^n -
 *          A027377(n).
 * https://oeis.org/A058819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n - I(n) (n≥2), where I(n) = (1/n) * Σ_{d|n} μ(d) *
 *            4^{n/d} a(0) = 1, a(1) = 4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static int  *mu      = NULL;   /* Möbius function up to current limit */
static size_t mu_len = 0;
static seq_t *pow4    = NULL;   /* 4^k for k=0..current limit */
static size_t pow4_len = 0;

static void
prepare(idx_t limit)
{
    if (limit + 1 <= mu_len && limit + 1 <= pow4_len)
        return;                  /* already prepared */

    /* reallocate arrays */
    mu      = (int *)realloc(mu,       (limit + 1) * sizeof(int));
    pow4    = (seq_t *)realloc(pow4,   (limit + 1) * sizeof(seq_t));
    if (!mu || !pow4)
        exit(1);                  /* memory error */

    /* compute Möbius function up to limit */
    mu[0] = 0;
    if (limit >= 1)
        mu[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        int temp = (int)i;
        int cnt  = 0;
        bool sq  = false;
        for (int p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                cnt++;
                temp /= p;
                if (temp % p == 0) { sq = true; break; }
                while (temp % p == 0) temp /= p;
            }
        }
        if (sq) { mu[i] = 0; continue; }
        if (temp > 1) cnt++;               /* remaining prime factor */
        mu[i] = (cnt % 2 == 0) ? 1 : -1;
    }

    /* compute powers of 4 */
    pow4[0] = (seq_t)1;
    for (idx_t i = 1; i <= limit; ++i)
        pow4[i] = pow4[i-1] * (seq_t)4;

    mu_len   = limit + 1;
    pow4_len = limit + 1;
}

seq_t a(idx_t n)
{
    /* special cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    prepare(n);                 /* ensure tables up to n are ready */

    seq_t total   = pow4[n];
    seq_t posSum  = (seq_t)0;   /* sum of 4^{n/d} with μ(d)=+1 */
    seq_t negSum  = (seq_t)0;   /* sum of 4^{n/d} with μ(d)=-1 */

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;
        idx_t d1 = i;
        idx_t d2 = n / i;

        int mu1 = mu[d1];
        seq_t val1 = pow4[n / d1];
        if (mu1 == 1) posSum += val1;
        else if (mu1 == -1) negSum += val1;

        if (d1 != d2) {
            int mu2 = mu[d2];
            seq_t val2 = pow4[n / d2];
            if (mu2 == 1) posSum += val2;
            else if (mu2 == -1) negSum += val2;
        }
    }

    seq_t diff        = posSum - negSum;      /* guaranteed non‑negative */
    seq_t irreducible = diff / (seq_t)n;
    return total - irreducible;
}
