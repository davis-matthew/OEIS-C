/*
 * A005213  Number of symmetric, reduced unit interval schemes with n+1
 *          intervals (n>=1).
 * https://oeis.org/A005213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k) = A002426(k) = Sum_{j=0..floor(k/2)} k!/(j!^2*(k-2j)!),
 *            a(2k+1) = A005717(k) = Sum_{j=0..floor((k-1)/2)}
 *            k!/(j!*(j+1)!*(k-2j-1)!)
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 45, u64 n <= 85, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* exact x*p/q where q divides x*p, without forming x*p */
static seq_t mul_div(seq_t x, idx_t p, idx_t q) {
    seq_t rs = x % q;
    idx_t r = (idx_t)(long long)rs;
    idx_t g = gcd_idx(q, r);
    x /= g;
    q /= g;
    return x * (p / q);
}

seq_t a(idx_t n) {
    idx_t k = n / 2;
    seq_t sum = 0;
    if (n % 2 == 0) {
        seq_t t = 1;
        idx_t j;
        sum = t;
        for (j = 0; 2 * j + 2 <= k; j++) {
            t = mul_div(t, k - 2 * j, j + 1);
            t = mul_div(t, k - 2 * j - 1, j + 1);
            sum += t;
        }
    } else {
        idx_t j;
        seq_t u;
        if (k == 0) return 0;
        u = (seq_t)k;
        sum = u;
        for (j = 1; 2 * j + 1 <= k; j++) {
            u = mul_div(u, k - 2 * j + 1, j + 1);
            u = mul_div(u, k - 2 * j, j);
            sum += u;
        }
    }
    return sum;
}
