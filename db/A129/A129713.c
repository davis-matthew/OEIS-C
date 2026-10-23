/*
 * A129713  Triangle read by rows: T(n,k) is the number of Fibonacci binary
 *          words of length n and starting with exactly k 1's (0<=k<=n). A
 *          Fibonacci binary word is a binary word having no 00 subword.
 * https://oeis.org/A129713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = max(1, F(m – d)) * where m = largest integer with
 *            m(m+1)/2 ≤ n (triangular index), * d = n – m(m+1)/2, * and F(k)
 *            is the k‑th Fibonacci number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 104, u16 n <= 324, u32 n <= 1175, u64 n <= 4464,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is OEIS A129713. */

#include <stdint.h>
#include <math.h>
#include <stdlib.h> /* for size_t */

static inline idx_t triangular_index(idx_t n)
{
    /* Find largest m such that m*(m+1)/2 ≤ n */
    idx_t lo = 0, hi = (idx_t)(sqrt((double)2.0 * n) + 2.0);

    /* Ensure hi is high enough */
    while (((__uint128_t)hi * (hi + 1)) / 2 <= n)
        hi <<= 1;

    /* Binary search */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (((__uint128_t)mid * (mid + 1)) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static seq_t fibonacci(idx_t k)
{
    if (k == 0)
        return 0;
    seq_t a = 0;
    seq_t b = 1;
    for (idx_t i = 2; i <= k; ++i) {
        seq_t c = a + b;
        a = b;
        b = c;
    }
    return (k == 1) ? b : (k == 0 ? a : b);
}

seq_t a(idx_t n)
{
    idx_t m = triangular_index(n);
    idx_t d = n - (idx_t)(((__uint128_t)m * (m + 1)) / 2);
    idx_t delta = (m >= d) ? (m - d) : 0U;   /* delta ≥ 0 */

    if (delta == 0U)
        return 1;                         /* zeros are replaced by 1 */

    return fibonacci(delta);
}
