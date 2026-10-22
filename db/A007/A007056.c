/*
 * A007056  Let S denote the palindromes in the language {0,1,2}*; a(n) =
 *          number of words of length n in the language SS.
 * https://oeis.org/A007056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A187273(n) - Sum_{d|n,d<n} phi(n/d)*a(d), a(0) = 1, where
 *            A187273(n) = Sum_{i=0}^{n-1} P(i)*P(n-i), P(k) = 3^ceil(k/2),
 *            i.e. A187273(n) = n*3^((n+1)/2) for odd n, 2n*3^(n/2) for even n
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   phi_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi_u.h"

#define MEMO_N 20001

static seq_t memo_a[MEMO_N];
static bool computed[MEMO_N];

static seq_t pow3(idx_t e) {
    seq_t r = 1, b = 3;
    while (e > 0) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e > 0) b *= b;
    }
    return r;
}

static seq_t A187273(idx_t n) {
    if (n % 2 == 1) return (seq_t)n * pow3((n + 1) / 2);
    return (seq_t)(2 * n) * pow3(n / 2);
}

static seq_t a_calc(idx_t n);

static seq_t a_get(idx_t n) {
    if (n < MEMO_N) {
        if (!computed[n]) {
            memo_a[n] = a_calc(n);
            computed[n] = true;
        }
        return memo_a[n];
    }
    return a_calc(n);
}

static seq_t a_calc(idx_t n) {
    if (n == 0) return 1;
    seq_t result = A187273(n);
    for (idx_t d = 1; d <= n / 2; d++) {
        if (n % d == 0) {
            result -= (seq_t)phi_u(n / d) * a_get(d);
        }
    }
    return result;
}

seq_t a(idx_t n) {
    return a_get(n);
}
