/*
 * A043600  Numbers whose base-4 representation has exactly 9 runs.
 * https://oeis.org/A043600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function enumerates all natural numbers whose base‑4 representation
   contains exactly 9 runs.  It stores the list of such numbers in increasing
   order (the same order as the OEIS sequence) and returns the n‑th element.
   Only the first 19683 terms are generated because the minimal
   representation has 9 digits (one digit per run), and
   19683 > 10000, which covers the requested range. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Recursive generation of 9‐digit base‑4 numbers with no repeated
   neighbouring digits and a leading digit of 1, 2 or 3. */
static void gen_rec(int depth, int prev, seq_t cur, seq_t *table, size_t *cnt,
                    size_t cap)
{
    if (depth == 9) {
        table[(*cnt)++] = cur;
        return;
    }
    for (int d = 0; d < 4; ++d) {
        if (d == prev) continue;
        gen_rec(depth + 1, d, (cur << 2) | (seq_t)d, table, cnt, cap);
    }
}

/* Compare function for qsort on seq_t values. */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n<1 */

    static bool initialized = false;
    static seq_t *table = NULL;
    static size_t table_size = 0;
    const size_t MAX_TERMS = 20000;       /* 19683 is the exact count */

    if (!initialized) {
        initialized = true;
        table = (seq_t *)malloc(MAX_TERMS * sizeof(seq_t));
        if (!table) return 0;             /* allocation failure */

        size_t cnt = 0;
        /* Generate all 9‑run numbers. */
        for (int first = 1; first <= 3; ++first) {
            gen_rec(1, first, (seq_t)first, table, &cnt, MAX_TERMS);
        }
        table_size = cnt;

        /* Sort to guarantee increasing order. */
        qsort(table, table_size, sizeof(seq_t), cmp_seq);
    }

    if ((idx_t)n > table_size) return 0;   /* request beyond precomputed list */
    return table[n - 1];
}
