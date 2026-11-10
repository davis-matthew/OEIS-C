/*
 * A076103  Sums of members of groups in A076105.
 * https://oeis.org/A076103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of row n of the triangle A076105 (row 1 = {1}; row k
 *            = the k-1 least unused naturals plus the least unused number
 *            making the row sum a multiple of the previous row sum); computed
 *            by direct simulation
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A076103_sums = NULL;   /* row sums, index r-1 */
static seq_t *A076103_lasts = NULL;  /* "last" elements of rows >= 2 */
static size_t A076103_cap = 0;
static size_t A076103_rows = 0;
static seq_t A076103_next = 2;       /* all numbers below this are used */

static int A076103_in_lasts(seq_t v)
{
    for (size_t i = 0; i < A076103_rows; i++) {
        if (A076103_lasts[i] == v) return 1;
    }
    return 0;
}

static void A076103_grow(size_t need)
{
    if (need <= A076103_cap) return;
    size_t nc = A076103_cap ? A076103_cap * 2 : 64;
    while (nc < need) nc *= 2;
    seq_t *s = mem_resize(seq_t, A076103_sums, nc);
    seq_t *l = mem_resize(seq_t, A076103_lasts, nc);
    if (!s || !l) abort();
    A076103_sums = s;
    A076103_lasts = l;
    A076103_cap = nc;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    A076103_grow((size_t)n + 1);
    while (A076103_rows < (size_t)n) {
        if (A076103_rows == 0) {
            A076103_sums[0] = 1;
            A076103_lasts[0] = 1;   /* 1 lies below next, harmless */
            A076103_rows = 1;
            continue;
        }
        size_t r = A076103_rows + 1;
        seq_t prev = A076103_sums[A076103_rows - 1];
        seq_t sum = 0;
        for (size_t i = 0; i + 1 < r; i++) {
            while (A076103_in_lasts(A076103_next)) A076103_next++;
            sum += A076103_next;
            A076103_next++;
        }
        seq_t rem = sum % prev;
        seq_t cand = (prev - rem) % prev;
        if (cand == 0) cand = prev;
        while (cand < A076103_next || A076103_in_lasts(cand)) cand += prev;
        A076103_lasts[A076103_rows] = cand;
        A076103_sums[A076103_rows] = sum + cand;
        A076103_rows++;
    }
    return A076103_sums[n - 1];
}
