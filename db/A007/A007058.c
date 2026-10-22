/*
 * A007058  Let S denote the palindromes in the language {0,1,2,3,4}*; a(n) =
 *          number of words of length n in the language SS.
 * https://oeis.org/A007058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A187275(n) - Sum_{d|n,d<n} phi(n/d)*a(d), a(0) = 1, where
 *            A187275(n) = n*5^((n+1)/2) for odd n and 3*n*5^(n/2) for even n
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/a000272_ipow.h"

static unsigned long long phi(unsigned long long n) {
    unsigned long long result = n;
    for (unsigned long long p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

static seq_t A187275(idx_t n) {
    if (n == 0) return 1;
    seq_t base = (seq_t)n;
    if ((n & 1) == 0) base = base * 3;
    return base * a000272_ipow((seq_t)5, (n + 1) / 2);
}

static seq_t memo[10001];
static bool computed[10001];

seq_t a(idx_t n) {
    if (n < 10001 && computed[n]) return memo[n];

    seq_t result = A187275(n);

    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            if (d < n) {
                result -= (seq_t)phi(n / d) * a(d);
            }
            idx_t d2 = n / d;
            if (d2 != d && d2 < n) {
                result -= (seq_t)phi(n / d2) * a(d2);
            }
        }
    }

    if (n < 10001) {
        memo[n] = result;
        computed[n] = true;
    }

    return result;
}
