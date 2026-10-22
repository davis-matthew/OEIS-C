/*
 * A006850  Exponential self-convolution of numbers of rooted trees on n
 *          nodes.
 * https://oeis.org/A006850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*A000081(k+1)*A000081(n-k+1),
 *            where A000081(1) = 1 and A000081(m) = (1/(m-1)) * Sum_{k=1..m-1}
 *            (Sum_{d|k} d*A000081(d)) * A000081(m-k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 55,
 *            big any n
 * Helpers:   alloc_zero, binom, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/binom.h"

static seq_t *T = NULL;   /* T[m] = A000081(m) */
static seq_t *S = NULL;   /* S[k] = Sum_{d|k} d*T[d] */
static idx_t tlen = 0;

static void ensure(idx_t need) {
    idx_t newlen, start, m, k, d;
    if (need < tlen) return;
    newlen = need + 1;
    if (newlen < 2 * tlen) newlen = 2 * tlen;
    if (T == NULL) {
        T = alloc_zero(newlen);
        S = alloc_zero(newlen);
        T[1] = 1;
        S[1] = 1;
        start = 2;
    } else {
        T = grow_zero(T, tlen, newlen);
        S = grow_zero(S, tlen, newlen);
        start = tlen;
    }
    for (m = start; m < newlen; m++) {
        seq_t sum = 0;
        seq_t div = (seq_t)(m - 1);
        for (k = 1; k < m; k++)
            sum += S[k] * T[m - k];
        T[m] = (div == 0) ? (seq_t)0 : sum / div;
        {
            seq_t s = 0;
            for (d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    idx_t e = m / d;
                    s += (seq_t)d * T[d];
                    if (e != d) s += (seq_t)e * T[e];
                }
            }
            S[m] = s;
        }
    }
    tlen = newlen;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t k;
    ensure(n + 1);
    for (k = 0; k <= n; k++)
        result += binom(n, k) * T[k + 1] * T[n - k + 1];
    return result;
}
