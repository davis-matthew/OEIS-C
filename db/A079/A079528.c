/*
 * A079528  a(n) = sigma(n) - ceiling(n + sqrt n).
 * https://oeis.org/A079528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = σ(n) – ⌈n + √n⌉ = σ(n) – n – ⌈√n⌉
 * Range:     i8 n <= 31, i16 n <= 2907, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required types seq_t and idx_t are provided by the build system. */

#include <stdint.h>

/* Integer square root (floor) for non‑negative values of type idx_t. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t sq = (seq_t)mid * (seq_t)mid;
        if (sq == n) return mid;
        if (sq < n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* Sum of divisors σ(n) computed by trial division. */
static seq_t sigma_u64(idx_t n)
{
    if (n == 0) return 0;               /* not expected for this sequence */
    if (n == 1) return (seq_t)1;         /* σ(1) = 1 */

    seq_t sum = 1;                       /* start with divisor 1 */
    idx_t d;
    for (d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += d;
            idx_t e = n / d;
            if (e != d) sum += e;       /* avoid double‑counting a square root */
        }
    }
    sum += (seq_t)n;                     /* add the divisor n itself */
    return sum;
}

seq_t a(idx_t n)
{
    /* Compute σ(n) */
    seq_t sig = sigma_u64(n);

    /* Compute ⌈√n⌉ */
    idx_t fl = isqrt_u64(n);                 /* floor sqrt */
    seq_t ceil_sqrt = (seq_t)fl;
    if ((seq_t)fl * (seq_t)fl != (seq_t)n)  /* not a perfect square? */
        ceil_sqrt += 1;

    /* a(n) = σ(n) – n – ⌈√n⌉ */
    seq_t n_seq = (seq_t)n;
    return sig - n_seq - ceil_sqrt;
}
