/*
 * A000706  Expansion of modular function 1/E_3 (cf. A013973).
 * https://oeis.org/A000706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 504 * Sum_{k=1..n} sigma_5(k) * a(n-k) for n >=
 *            1 (1/E_6, where E_6 = 1 - 504*Sum_{k>=1} sigma_5(k) q^k is
 *            A013973)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 356 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t a706_sigma5(idx_t m) {
    seq_t sum = 0;
    idx_t d;
    for (d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            seq_t x = (seq_t)d;
            sum += x * x * x * x * x;
            if (d * d != m) {
                seq_t y = (seq_t)(m / d);
                sum += y * y * y * y * y;
            }
        }
    }
    return sum;
}
#define A706_MAX 20001

static seq_t a706_memo[A706_MAX];
static seq_t a706_s5[A706_MAX];
static idx_t a706_done = 0;   /* number of a-values computed */
static idx_t a706_s5done = 1; /* sigma5 computed for indices < this */

seq_t a(idx_t n) {
    if (n >= A706_MAX) return 0;
    while (a706_s5done <= n) {
        a706_s5[a706_s5done] = a706_sigma5(a706_s5done);
        a706_s5done++;
    }
    while (a706_done <= n) {
        idx_t m = a706_done;
        if (m == 0) {
            a706_memo[0] = (seq_t)1;
        } else {
            seq_t sum = 0;
            idx_t k;
            for (k = 1; k <= m; k++)
                sum += a706_s5[k] * a706_memo[m - k];
            a706_memo[m] = sum * 504;
        }
        a706_done++;
    }
    return a706_memo[n];
}
