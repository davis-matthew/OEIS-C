/*
 * A176206  Irregular triangle T(n,k) (n >= 1, k >= 1) read by rows: row n has
 *          length A000070(n-1) and every column k gives the positive
 *          integers.
 * https://oeis.org/A176206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the n-th term of an irregular triangle T where row r
 *            contains the number (r‑k) repeated p(k) times (p = partition
 *            numbers). The first few rows are: 1; 2,1; 3,2,1,1;
 *            4,3,2,2,1,1,1; etc. The sequence a(n) is obtained by reading the
 *            triangle row by row
 * Range:     u8 n <= 9033, u16 n <= 9033, u32 n <= 9033, u64 n <= 9033,
 *            u128 n <= 9033, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define MAX_K 200 /* sufficient for n up to ~10 000 */

seq_t a(idx_t n) {
    static bool initialized = false;
    static uint64_t p[MAX_K + 1];
    static uint64_t sumP[MAX_K + 1];

    if (!initialized) {
        /* compute partition numbers p[0..MAX_K] via the pentagonal theorem */
        p[0] = 1;
        for (idx_t i = 1; i <= MAX_K; ++i) {
            int64_t total = 0;
            for (idx_t k = 1;; ++k) {
                idx_t pent1 = k * (3 * k - 1) / 2;
                idx_t pent2 = k * (3 * k + 1) / 2;
                if (pent1 > i && pent2 > i) break;
                int sign = (k % 2) ? 1 : -1;
                if (pent1 <= i) total += sign * (int64_t)p[i - pent1];
                if (pent2 <= i) total += sign * (int64_t)p[i - pent2];
            }
            p[i] = (uint64_t)total;
        }
        /* prefix sums of p for row lengths */
        sumP[0] = p[0];
        for (idx_t i = 1; i <= MAX_K; ++i) sumP[i] = sumP[i - 1] + p[i];
        initialized = true;
    }

    /* locate the row containing index n */
    idx_t total = 0;          /* total elements processed so far */
    idx_t row   = 1;          /* current row number (1‑based) */
    while (true) {
        idx_t row_len = sumP[row - 1];
        if (total + row_len >= n) break;
        total += row_len;
        ++row;
    }

    /* position inside the identified row (0‑based) */
    idx_t offset = n - total - 1;

    /* find the segment containing the offset */
    idx_t k = 0;
    idx_t prefix = 0;
    while (true) {
        idx_t count = p[k];
        if (offset < prefix + count) {
            return (seq_t)(row - k);
        }
        prefix += count;
        ++k;
    }
}
