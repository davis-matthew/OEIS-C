/*
 * A206438  Triangle read by rows which lists the squares of the parts of
 *          A135010.
 * https://oeis.org/A206438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A135010(n)^2, where A135010 is the triangle whose row r
 *            lists p(r-1) ones, then the parts of the partitions of r with no
 *            part equal to 1 (lexicographic order)
 * Range:     u8 n <= 1462, u16 n <= 10206, u32 n <= 10206, u64 n <= 10206,
 *            u128 n <= 10206, big any n
 * Validated: standard, pass, 99 terms through n = 9582 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool A135010_find_partitions(idx_t *rem, seq_t *out,
                                    int sum, int min_part,
                                    int *parts, int depth)
{
    if (sum == 0) {
        for (int i = 0; i < depth; ++i) {
            if (--(*rem) == 0) {
                *out = (seq_t)parts[i];
                return true;
            }
        }
        return false;
    }
    for (int part = min_part; part <= sum; ++part) {
        parts[depth] = part;
        if (A135010_find_partitions(rem, out, sum - part, part, parts, depth + 1))
            return true;
    }
    return false;
}

static seq_t *A135010_p_parts = NULL;
static idx_t A135010_p_size = 0;

static void A135010_ensure_partitions(idx_t limit)
{
    if (limit < 200) limit = 200;
    if (A135010_p_parts && A135010_p_size > limit) return;

    seq_t *new_table = mem_resize(seq_t, A135010_p_parts, (limit + 1));
    if (!new_table) abort();
    A135010_p_parts = new_table;
    A135010_p_size = limit + 1;

    A135010_p_parts[0] = 1;
    for (idx_t i = 1; i <= limit; ++i) A135010_p_parts[i] = 0;
    for (idx_t k = 1; k <= limit; ++k)
        for (idx_t i = k; i <= limit; ++i)
            A135010_p_parts[i] += A135010_p_parts[i - k];
}

static seq_t A135010_a(idx_t n)
{
    A135010_ensure_partitions(200);

    idx_t remaining = n;
    for (idx_t r = 1;; ++r) {
        idx_t ones = (r - 1 <= 200) ? (idx_t)A135010_p_parts[r - 1] : 0;
        if (remaining <= ones)
            return (seq_t)1;
        remaining -= ones;

        int parts_buf[128];
        seq_t value = 0;
        if (A135010_find_partitions(&remaining, &value, (int)r, 2, parts_buf, 0))
            return value;
    }
}

seq_t a(idx_t n)
{
    seq_t v = A135010_a(n);
    return v * v;
}
