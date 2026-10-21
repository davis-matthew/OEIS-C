/*
 * A081304  Number of numbers m <= n with prime factors less than 2*lpf(m),
 *          where lpf(m) is the least prime factor of m (A020639).
 * https://oeis.org/A081304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of integers m ≤ n whose prime factors are all < 2 *
 *            lpf(m)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* lpf(m) is the least prime factor of m. 1 is considered to satisfy the condition. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: test if m satisfies the condition using pre‑computed spf array */
static bool valid_m(idx_t m, const idx_t *spf)
{
    if (m == 1) return true;                     /* 1 has no prime factors */
    idx_t minp   = spf[m];                       /* least prime factor */
    idx_t threshold = minp + minp;               /* 2 * minp, fits in idx_t for n ≤ 10^4 */

    idx_t x = m;
    while (x > 1) {
        idx_t p = spf[x];
        if (p >= threshold) return false;        /* found a prime ≥ 2*minp */
        while (x % p == 0) x /= p;               /* divide all copies of p */
    }
    return true;
}

/* Core sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined for n=0; return 0 */

    /* Allocate array for smallest prime factor (spf) up to n */
    idx_t *spf = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!spf) return 0;                         /* allocation failure */

    for (idx_t i = 0; i <= n; ++i) spf[i] = i;   /* spf[i] = i initially */

    /* Sieve to compute lpf for each number */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (spf[i] == i) {                      /* i is prime */
            for (idx_t j = i * i; j <= n; j += i) {
                if (spf[j] == j) spf[j] = i;    /* set least prime factor once */
            }
        }
    }

    seq_t count = 0;
    for (idx_t m = 1; m <= n; ++m) {
        if (valid_m(m, spf)) ++count;
    }

    free(spf);
    return count;
}
