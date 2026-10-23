/*
 * A043318  a(n)=A033012(n)/15.
 * https://oeis.org/A043318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A033012(n)/15 (numbers whose base‑196 digits are <14 *
 *            and no two adjacent digits are equal, divided by 15). The
 *            function * generates all such numbers up to four base‑196 digits
 *            (enough for the * first 30940 terms, covering n up to ≈10000),
 *            sorts them, and returns * the nth smallest (1‑based)
 * Range:     u8 none, u16 none, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int cmp_seq_t(const void *a, const void *b) {
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n) {
    /* 4‑digit (in base 196) numbers with digits 0–13, without equal
     * consecutive digits, and leading digit non‑zero.  There are
     * 13 + 13² + 13³ + 13⁴ = 12 940 such numbers.  */
    static seq_t table[30940];
    static size_t count = 0;
    static int initialized = 0;

    if (!initialized) {
        size_t idx = 0;
        for (int d0 = 1; d0 <= 13; ++d0) {
            seq_t v0 = (seq_t)d0;
            table[idx++] = v0;

            for (int d1 = 0; d1 <= 13; ++d1) {
                if (d1 == d0) continue;
                seq_t v1 = v0 * (seq_t)196 + (seq_t)d1;
                table[idx++] = v1;

                for (int d2 = 0; d2 <= 13; ++d2) {
                    if (d2 == d1) continue;
                    seq_t v2 = v1 * (seq_t)196 + (seq_t)d2;
                    table[idx++] = v2;

                    for (int d3 = 0; d3 <= 13; ++d3) {
                        if (d3 == d2) continue;
                        seq_t v3 = v2 * (seq_t)196 + (seq_t)d3;
                        table[idx++] = v3;
                    }
                }
            }
        }
        count = idx;   /* 30940 */

        /* Sort the generated numbers to obtain the increasing order. */
        qsort(table, count, sizeof(seq_t), cmp_seq_t);
        initialized = 1;
    }

    /* No explicit handling for n > count is required for the intended
     * usage (n up to about 10000).  If n is out of range, returning
     * 0 is safe, but callers should supply n ≥ 1 and n ≤ count. */
    if (n == 0 || n > count) return (seq_t)0;
    return table[n - 1];
}
