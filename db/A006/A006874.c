/*
 * A006874  Number of mu-atoms of period n on continent of Mandelbrot set.
 * https://oeis.org/A006874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{ d divides n, d<n} phi(n/d)*a(d), n>1, a(1)=1
 * Range:     u8 n <= 47, u16 n <= 719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned long long phi(unsigned long long n) {
    unsigned long long result = n;
    for (unsigned long long p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) {
                n /= p;
            }
            result -= result / p;
        }
    }
    if (n > 1) {
        result -= result / n;
    }
    return result;
}

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n == 1) return 1;
    
    if (n >= memo_len) {
        idx_t new_len = (n < 100) ? 100 : (n * 3 / 2 + 1);
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (memo) {
            for (idx_t i = 0; i < memo_len; i++) {
                new_memo[i] = memo[i];
            }
            free(memo);
        }
        for (idx_t i = memo_len; i < new_len; i++) {
            new_memo[i] = 0;
        }
        memo = new_memo;
        memo_len = new_len;
    }
    
    if (memo[n] != 0) return memo[n];
    
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            if (d < n) {
                unsigned long long p = phi((unsigned long long)(n / d));
                result += (seq_t)p * a(d);
            }
            idx_t d2 = n / d;
            if (d2 != d && d2 < n) {
                unsigned long long p = phi((unsigned long long)(n / d2));
                result += (seq_t)p * a(d2);
            }
        }
    }
    
    memo[n] = result;
    return result;
}
