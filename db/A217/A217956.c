/*
 * A217956  Total number of solutions to the equation x^2+k*y^2 = n with x >
 *          0, y > 0, k >= 0, or 0 if infinite. (Order matters for the
 *          equation x^2+y^2 = n).
 * https://oeis.org/A217956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to x^2 + k·y^2 = n with x>0, y>0, k≥0
 *            (ordered (x,y) counted separately). If n is a perfect square the
 *            number of solutions is infinite, so the sequence returns 0 in
 *            that case
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of integers y ≥ 1 with y² | m.
   This equals the number of square divisors of m. */
static seq_t sq_div_count(idx_t m) {
    seq_t count = 1;
    idx_t exp = 0;
    /* factor 2 */
    while ((m & 1) == 0) {
        exp++;
        m >>= 1;
    }
    if (exp) count *= (exp / 2 + 1);
    /* factor odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        exp = 0;
        while (m % p == 0) {
            exp++;
            m /= p;
        }
        if (exp) count *= (exp / 2 + 1);
    }
    /* remaining prime factor (exponent 1) contributes 1, no change */
    return count;
}
/* Note: sz_t and idx_t are supplied by the build; do not define them. */

static idx_t isqrt(idx_t n) {
    if (n < 2) return n;
    idx_t left  = 1;
    idx_t right = ((idx_t)1 << ((sizeof(idx_t) * 8) / 2));  /* ≥ √max(idx_t) */
    while (left <= right) {
        idx_t mid = (left + right) >> 1;
        if (mid <= n / mid) {          /* mid*mid ≤ n, avoid overflow */
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return right;
}

seq_t a(idx_t n) {
    /* Infinite solutions when n is a perfect square. */
    idx_t s = isqrt(n);
    if (s * s == n) return 0;
    idx_t limit = isqrt(n - 1);          /* x must satisfy x² < n */
    seq_t total = 0;
    for (idx_t x = 1; x <= limit; ++x) {
        idx_t rem = n - x * x;           /* rem > 0 because x² < n */
        total += sq_div_count(rem);
    }
    return total;
}
