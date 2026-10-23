/*
 * A141618  Triangle read by rows: number of nilpotent partial transformations
 *          (of an n-element set) of height r (height(alpha) = |Im(alpha)|), 0
 *          <= r < n.
 * https://oeis.org/A141618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(row,col) * S(row,col+1) * col! (S = Stirling numbers of
 *            2nd kind) Equivalent: a(n) = C(row,col) * (1/(col+1)) *
 *            Σ_{i=0}^{col+1} (-1)^i * C(col+1,i) * (col+1-i)^row where
 *            (row,col) is the position of a(n) in the triangle read by rows:
 *            row = smallest r with r(r+1)/2 ≥ n; col = n – r(r-1)/2...
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t _isqrt_u64(idx_t x)
{
    /* integer sqrt floor using binary search */
    idx_t lo = 0, hi = x, res = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) { res = 0; lo = 1; continue; }
        if (mid > x / mid) { /* mid*mid > x */
            hi = mid - 1;
        } else {
            res = mid;
            lo = mid + 1;
        }
    }
    return res;
}

static void _index_to_rc(idx_t idx, idx_t *row, idx_t *col)
{
    /* find smallest r with r(r+1)/2 ≥ idx */
    idx_t t = (idx_t)8 * idx + 1;
    idx_t s = _isqrt_u64(t);
    idx_t r = (s - 1) >> 1;
    if (((unsigned __int128)r * (r + 1)) / 2 < idx) r++;
    *row = r;
    idx_t prev = ((unsigned __int128)r * (r - 1)) / 2;
    *col = idx - prev - 1;
}

/* binomial coefficient C(n,k) using multiplicative algorithm */
static seq_t _binom_nk(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;            /* exact division */
    }
    return res;
}

/* integer exponentiation: base^exp (exp fits in int) */
static seq_t _pow_seq(seq_t base, int exp)
{
    seq_t result = 1;
    seq_t b = base;
    int e = exp;
    while (e > 0) {
        if (e & 1) result = result * b;
        b = b * b;
        e >>= 1;
    }
    return result;
}

/* Public sequence function required by the build. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t row, col;
    _index_to_rc(n, &row, &col);

    seq_t binom_n_r = _binom_nk(row, col);
    idx_t k = col + 1;                     /* k = r + 1 */
    seq_t sum = 0;

    for (idx_t i = 0; i <= k; ++i) {
        seq_t binom_k_i = _binom_nk(k, i);
        seq_t power = _pow_seq((seq_t)(k - i), (int)row);
        seq_t term = binom_k_i * power;
        if ((i & 1) == 0) {
            sum += term;
        } else {
            sum -= term;
        }
    }

    /* divide the alternating sum by k = col+1 */
    sum = sum / (seq_t)k;

    return binom_n_r * sum;
}
