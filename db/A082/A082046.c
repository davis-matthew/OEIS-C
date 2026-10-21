/*
 * A082046  Square array, A(n, k) = (k*n)^2 + 3*k*n + 1, read by
 *          antidiagonals.
 * https://oeis.org/A082046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (i * (m - i))^2 + 3 * i * (m - i) + 1 where m = the
 *            smallest non‑negative integer such that (m + 1)(m + 2)/2 > n,
 *            and i = n - m(m + 1)/2. This reads the symmetric square array
 *            A(n, k) = (n * k)^2 + 3 * n * k + 1 by antidiagonals
 * Range:     u8 n <= 38, u16 n <= 542, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find diagonal m via incremental search (O(sqrt(n)) steps). */
    idx_t m = 0;
    idx_t sum = 1;                 /* number of terms up to and including current m */
    while (sum <= n) {
        ++m;
        sum += (m + 1);
    }

    /* Previous total (before current diagonal). */
    __int128 prev128 = (__int128)m * (m + 1) / 2;
    idx_t prev = (idx_t)prev128;    /* fits in idx_t because prev <= n */

    /* Offset within diagonal m. */
    idx_t i = n - prev;
    idx_t k = m - i;

    /* Compute the value. */
    seq_t prod = (seq_t)i * (seq_t)k;        /* i * k */
    seq_t val  = prod * prod + 3 * prod + 1; /* (i*k)^2 + 3*i*k + 1 */

    return val;
}
