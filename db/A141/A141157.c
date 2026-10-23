/*
 * A141157  Triangle read by rows, A000012 * A140207.
 * https://oeis.org/A141157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r - c + 1) * p(c) where r = ⌊(√(8n+1) - 1)/2⌋ (row
 *            index) c = n - r(r+1)/2 (column index) p(k) = number of
 *            partitions of k (A000041)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partition numbers are cached in a static array, computed once per run. */
#include <stdlib.h>

/* part[k] = p(k) for k = 0 .. part_len-1   */
static seq_t *part = NULL;
static idx_t part_len = 0;

/* Compute partition numbers up to max_n (inclusive). */
static void ensure_partitions(idx_t max_n)
{
    if (max_n + 1 <= part_len)
        return;                       /* already have enough values */

    size_t new_len = (size_t)(max_n + 1);
    part = (seq_t *)realloc(part, new_len * sizeof(seq_t));

    /* initialize first element if array was empty */
    if (part_len == 0) {
        part[0] = 1;
        part_len = 1;
    }

    /* compute partition numbers for k = part_len .. max_n */
    for (idx_t n = part_len; n < new_len; ++n) {
        seq_t sum = 0;
        for (idx_t m = 1;; ++m) {
            idx_t g1 = m * (3 * m - 1) / 2;        /* generalized pentagonal */
            if (g1 > n) break;
            if (m & 1) sum += part[n - g1];
            else       sum -= part[n - g1];

            idx_t g2 = m * (3 * m + 1) / 2;
            if (g2 > n) continue;
            if (m & 1) sum += part[n - g2];
            else       sum -= part[n - g2];
        }
        part[n] = sum;
    }
    part_len = new_len;
}

/* ------------------------------------------------------------------ */
/* Compute the nth term of OEIS sequence A141157 (triangle read by rows) */
seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* triangular number of this row: r(r+1)/2 */
    idx_t prev = r * (r + 1) / 2;

    /* column index within the row */
    idx_t c = n - prev;

    /* ensure we have partition numbers up to r (so p(c) is available) */
    ensure_partitions(r);

    /* value = (r - c + 1) * p(c) */
    seq_t factor = (seq_t)(r - c + 1);
    seq_t value  = factor * part[c];

    return value;
}
