/*
 * A006605  Number of modes of connections of 2n points.
 * https://oeis.org/A006605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(2*n+1)) * Sum_{t=0..floor(n/2)}
 *            binomial(2*n+1,t)*binomial(2*n+1-t,n-2*t) i.e. a(n) = [x^n]
 *            (1+x+x^2)^(2*n+1) / (2*n+1). Terms T(t) =
 *            (2n+1)!/(t!(n-2t)!(n+1+t)!), T(0) = binomial(2n+1,n), T(t+1) =
 *            T(t)*(n-2t)*(n-2t-1)/((t+1)*(n+2+t)). For fixed-width seq_t the
 *            work is exact modulo 2^w...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t mulw(seq_t x, seq_t y) { return (seq_t)(x * 1u * y); }

static seq_t inv_odd(seq_t x) {
    seq_t y = x;
    for (int i = 0; i < 8; i++)
        y = mulw(y, (seq_t)(2 - mulw(x, y)));
    return y;
}

static void mul_q(seq_t *odd, int *e, uint64_t q) {
    while ((q & 1) == 0) { q >>= 1; (*e)++; }
    *odd = mulw(*odd, (seq_t)q);
}

static void div_q(seq_t *odd, int *e, uint64_t q) {
    while ((q & 1) == 0) { q >>= 1; (*e)--; }
    *odd = mulw(*odd, inv_odd((seq_t)q));
}

seq_t a(idx_t n) {
    seq_t m = 0;
    m--;
    bool fixed = m > 0;

    if (!fixed) {
        seq_t T = 1;
        for (idx_t i = 0; i < n; i++)
            T = T * (seq_t)(2 * n + 1 - i) / (seq_t)(i + 1);
        seq_t sum = 0;
        for (idx_t t = 0; 2 * t <= n; t++) {
            sum += T;
            if (2 * (t + 1) > n) break;
            T = T * (seq_t)(n - 2 * t) * (seq_t)(n - 2 * t - 1)
                / ((seq_t)(t + 1) * (seq_t)(n + 2 + t));
        }
        return sum / (seq_t)(2 * n + 1);
    }

    int w = 0;
    while (m != 0) { m >>= 1; w++; }

    seq_t odd = 1;
    int e = 0;
    for (idx_t i = 0; i < n; i++) {
        mul_q(&odd, &e, 2 * n + 1 - i);
        div_q(&odd, &e, i + 1);
    }
    seq_t sum = 0;
    for (idx_t t = 0; 2 * t <= n; t++) {
        if (e < w) sum = (seq_t)(sum + (seq_t)(odd << e));
        if (2 * (t + 1) > n) break;
        mul_q(&odd, &e, n - 2 * t);
        mul_q(&odd, &e, n - 2 * t - 1);
        div_q(&odd, &e, t + 1);
        div_q(&odd, &e, n + 2 + t);
    }
    return mulw(sum, inv_odd((seq_t)(2 * n + 1)));
}
