/*
 * A143317  Triangle read by rows: A051731 * A143239.
 * https://oeis.org/A143317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = T(r,k) where r = row, k = column. T(r,k)=mu(k)*sigma(r/k)
 *            if k|r else 0; row sums equal r
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* sigma(m) = sum of divisors of m */
static seq_t sigma_of(idx_t m) {
    seq_t sum = 0;
    for (idx_t i = 1; (unsigned __int128)i * i <= m; ++i) {
        if (m % i == 0) {
            sum += (seq_t)i;
            idx_t j = m / i;
            if (j != i) sum += (seq_t)j;
        }
    }
    return sum;
}

/* Möbius function mu(n) for n >= 1 */
static int mu_of(idx_t n) {
    if (n == 1) return 1;
    int count = 0;
    for (idx_t d = 2; (unsigned __int128)d * d <= n; ++d) {
        if (n % d == 0) {
            int exp = 0;
            while (n % d == 0) {
                n /= d;
                ++exp;
                if (exp > 1) return 0;   /* square prime factor */
            }
            ++count;
        }
    }
    if (n > 1) ++count;   /* remaining prime factor > sqrt */
    return (count % 2 == 0) ? 1 : -1;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Find the row r such that n lies in that row */
    idx_t r = 0;
    while (((unsigned __int128)r * (r + 1)) / 2 < n) ++r;

    /* Compute the starting index of this row */
    idx_t offset = (idx_t)(((unsigned __int128)(r - 1) * r) / 2 + 1);

    /* Column index (1‑based) */
    idx_t k = n - offset + 1;

    if (r % k != 0) return (seq_t)0;   /* entry is zero when k does not divide r */

    idx_t m = r / k;                    /* argument for sigma */
    seq_t sigma_val = sigma_of(m);
    int mu_k = mu_of(k);

    return (seq_t)mu_k * sigma_val;
}
