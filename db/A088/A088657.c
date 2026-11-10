/*
 * A088657  Except for initial 0, same as A005213.
 * https://oeis.org/A088657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0) = 0; for n >= 1, a(n) = A005213(n), where a(2k) =
 *            Sum_{j=0..floor(k/2)} k!/(j!^2*(k-2j)!), a(2k+1) =
 *            Sum_{j=0..floor((k-1)/2)} k!/(j!*(j+1)!*(k-2j-1)!)
 * Range:     i8 n <= 11, i16 n <= 20, i32 n <= 20, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A005213_gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* exact x*p/q where q divides x*p, without forming x*p */
static seq_t A005213_mul_div(seq_t x, idx_t p, idx_t q) {
    seq_t rs = x % q;
    idx_t r = (idx_t)(long long)rs;
    idx_t g = A005213_gcd_idx(q, r);
    x /= g;
    q /= g;
    return x * (p / q);
}

static seq_t A005213_a(idx_t n) {
    idx_t k = n / 2;
    seq_t sum = 0;
    if (n % 2 == 0) {
        seq_t t = 1;
        idx_t j;
        sum = t;
        for (j = 0; 2 * j + 2 <= k; j++) {
            t = A005213_mul_div(t, k - 2 * j, j + 1);
            t = A005213_mul_div(t, k - 2 * j - 1, j + 1);
            sum += t;
        }
    } else {
        idx_t j;
        seq_t u;
        if (k == 0) return 0;
        u = (seq_t)k;
        sum = u;
        for (j = 1; 2 * j + 1 <= k; j++) {
            u = A005213_mul_div(u, k - 2 * j + 1, j + 1);
            u = A005213_mul_div(u, k - 2 * j, j);
            sum += u;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    return A005213_a(n);
}
