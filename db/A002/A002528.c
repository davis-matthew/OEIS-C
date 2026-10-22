/*
 * A002528  a(n) = A188491(n+1) - A188494(n) - A002526(n).
 * https://oeis.org/A002528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.
 *            -2*x^2/((x-1)*(x^13+3x^12+3x^11+5x^10+9x^9+7x^8-3x^7-19x^6-21x^5-13x^4-3x^3-3x^2-x+1)).
 *            a(n) = Sum_{i=0..n} b(i), where b(n) =
 *            b(n-1)+3b(n-2)+3b(n-3)+13b(n-4)+21b(n-5)+19b(n-6)+3b(n-7)
 *            -7b(n-8)-9b(n-9)-5b(n-10)-3b(n-11)-3b(n-12)-b(n-13) + 2*[n==2],
 *            b(n) = 0 for n < 2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const long long A002528_pc[14] = {0, 1, 3, 3, 13, 21, 19, 3, 0, 0, 0, 0, 0, 0};
static const long long A002528_nc[14] = {0, 0, 0, 0, 0, 0, 0, 0, 7, 9, 5, 3, 3, 1};

seq_t a(idx_t n) {
    seq_t w[14]; /* w[k] = b(i-k) for k = 1..13 */
    seq_t sum = 0;
    idx_t i;
    int k;
    for (k = 0; k < 14; k++) w[k] = 0;
    for (i = 0; i <= n; i++) {
        seq_t pos = 0, neg = 0, b;
        for (k = 1; k < 14; k++) {
            if (A002528_pc[k]) pos += w[k] * (seq_t)A002528_pc[k];
            if (A002528_nc[k]) neg += w[k] * (seq_t)A002528_nc[k];
        }
        if (i == 2) pos += 2;
        b = pos - neg;
        sum += b;
        for (k = 13; k > 1; k--) w[k] = w[k - 1];
        w[1] = b;
    }
    return sum;
}
