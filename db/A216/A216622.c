/*
 * A216622  Square array read by antidiagonals: T(n,k) = Sum_{c|n, d|k}
 *          phi(lcm(c,d)) for n >= 1, k >= 1.
 * https://oeis.org/A216622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where i,j are the coordinates of the n‑th
 *            antidiagonal element of the square array T(n,k) = Σ_{c|n,d|k}
 *            φ(lcm(c,d))
 * Range:     u8 n <= 508, u16 n <= 10011, u32 n <= 10011, u64 n <= 10011,
 *            u128 n <= 10011, big any n
 * Validated: standard, pass, 87 terms through n = 9618 (2026-11-01)
 * Helpers:   gcd, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/totient_seq.h"

static void get_divisors(idx_t n, idx_t *buf, idx_t *len)
{
    idx_t cnt = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            buf[cnt++] = d;
            if (d != n / d)
                buf[cnt++] = n / d;
        }
    }
    *len = cnt;
}

static seq_t compute_T(idx_t i, idx_t j)
{
    idx_t di[128], dj[128];
    idx_t li, lj;
    get_divisors(i, di, &li);
    get_divisors(j, dj, &lj);

    seq_t sum = 0;
    for (idx_t a = 0; a < li; ++a) {
        for (idx_t b = 0; b < lj; ++b) {
            seq_t c = (seq_t)di[a];
            seq_t d = (seq_t)dj[b];
            seq_t lcm = (c / gcd(c, d)) * d;
            sum += totient_seq(lcm);
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal index s and position p within that diagonal. */
    idx_t s = 1;
    while (s * (s - 1) / 2 < n)
        ++s;                 /* s >= 2 because n >= 1 */

    idx_t prev_terms = (s - 1) * (s - 2) / 2;   /* terms before this diagonal */
    idx_t p = n - prev_terms - 1;              /* zero‑based position in the diagonal */

    idx_t i = p + 1;           /* row index */
    idx_t j = s - i;           /* column index */

    return compute_T(i, j);
}
