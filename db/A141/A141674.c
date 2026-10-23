/*
 * A141674  Triangle T(n,m) read by rows: T(n,m) = sigma_0(n) * A126988(n,m).
 * https://oeis.org/A141674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   a(n) = σ₀(r) * ( r if m = 1 r/m if r % m = 0 0 otherwise ) with
 *            r = smallest integer with r(r+1)/2 ≥ n, m = n – r(r-1)/2
 *            (1‑based position in row r)
 * Range:     u8 n <= 44, u16 n <= 5049, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t divisor_count(idx_t n) {
    seq_t cnt = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            if (i == n / i) cnt += 1;
            else             cnt += 2;
        }
    }
    return cnt;
}

seq_t a(idx_t n) {
    /* find r such that r(r+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t mid_sum = (seq_t)mid * (mid + 1) / 2;
        if (mid_sum >= n) high = mid;
        else              low = mid + 1;
    }
    idx_t r = low;
    /* offset = r(r-1)/2, position m in row */
    seq_t offset = (seq_t)r * (r - 1) / 2;
    idx_t m = (idx_t)(n - offset);  /* 1‑based */

    seq_t sigma0 = divisor_count(r);
    seq_t result;

    if (m == 1)
        result = sigma0 * r;
    else if (r % m == 0)
        result = sigma0 * (r / m);
    else
        result = 0;

    return result;
}
