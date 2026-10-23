/*
 * A159465  Sums of odd numbers, omitting squares.
 * https://oeis.org/A159465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+S)^2 - (4*S^3-S)/3 where S =
 *            ceil(round(sqrt(2*n-1))/2)
 * Range:     u8 n <= 14, u16 n <= 248, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, behaviour for 0 is unused */

    idx_t m = 2 * n - 1;                  /* 2*n - 1 */

    idx_t f = isqrt_u64(m);               /* floor(sqrt(m)) */
    idx_t f2 = f * f;
    idx_t diff_low = m - f2;

    idx_t next = f + 1;
    idx_t next2 = next * next;
    idx_t diff_high = next2 - m;

    idx_t round_s = (diff_low >= diff_high) ? next : f;  /* nearest integer to sqrt(m) */

    idx_t S = (round_s + 1) / 2;          /* ceil(round_s / 2) */

    seq_t sum = (seq_t)n + (seq_t)S;      /* n + S */
    seq_t sum_sq = sum * sum;             /* (n+S)^2 */

    seq_t Ssq = (seq_t)S * (seq_t)S;      /* S^2 */
    seq_t Scube = Ssq * (seq_t)S;         /* S^3 */
    seq_t val2 = (seq_t)4 * Scube - (seq_t)S; /* 4*S^3 - S */
    seq_t val2_div3 = val2 / 3;           /* divide by 3, exact */

    return sum_sq - val2_div3;
}
