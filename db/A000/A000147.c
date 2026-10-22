/*
 * A000147  Number of trees of diameter 5.
 * https://oeis.org/A000147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor((n-1)/2)} b(k)*b(n-k) + [n
 *            even]*C(b(n/2)+1, 2), where b(k) = P(k-1) - 1 = A000065(k-1),
 *            with P = partition numbers (A000041), computed by Euler's
 *            pentagonal recurrence P(m) = Sum_{j>=1} (-1)^(j+1)
 *            (P(m-j(3j-1)/2) + P(m-j(3j+1)/2)), P(0) = 1
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 72, u64 n <= 224,
 *            u128 n <= 761, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    if (n < 6) return res;
    idx_t m, k;
    seq_t *P = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    P[0] = 1;
    for (m = 1; m < n; m++) {
        seq_t pos = 0, neg = 0;
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (k % 2 == 1) {
                pos += P[m - g1];
                if (g2 <= m) pos += P[m - g2];
            } else {
                neg += P[m - g1];
                if (g2 <= m) neg += P[m - g2];
            }
        }
        P[m] = pos - neg;
    }
    /* b(k) = P[k-1] - 1 */
    idx_t half = (n % 2 == 1) ? (n - 1) / 2 : n / 2 - 1;
    for (k = 1; k <= half; k++) {
        seq_t bk = P[k - 1] - 1;
        seq_t bnk = P[n - k - 1] - 1;
        res += bk * bnk;
    }
    if (n % 2 == 0) {
        seq_t x = P[n / 2 - 1] - 1;
        seq_t c;
        if (x % 2 == 0) c = (x / 2) * (x + 1);
        else c = x * ((x + 1) / 2);
        res += c;
    }
    free(P);
    return res;
}
