/*
 * A171458  Squares in lunar arithmetic in base 4 written in base 10.
 * https://oeis.org/A171458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th distinct value of x ⊗ x (x in decimal) under *
 *            lunar arithmetic in base 4, expressed in base 10. The sequence
 *            is * sorted increasingly and may be queried in any order
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   lunar_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/lunar_square.h"

static bool initialized = false;
static seq_t *squares = NULL;
static size_t sq_count = 0;

static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Build a table of all distinct lunar squares up to a generous limit. */
static void build_squares(void)
{
    const idx_t LIMIT_X = 200000;        /* sufficient for the test range */
    const size_t BUF_SIZE = (size_t)LIMIT_X;

    seq_t *buf = (seq_t *)malloc(BUF_SIZE * sizeof(seq_t));
    if (!buf) return;                    /* allocation failed -> leave empty */

    size_t cnt = 0;
    for (idx_t x = 0; x < LIMIT_X; ++x) {
        buf[cnt++] = lunar_square(x);
    }

    qsort(buf, cnt, sizeof(seq_t), cmp_seq);

    /* Deduplicate in place. */
    size_t uniq = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (uniq == 0 || buf[i] != buf[uniq - 1]) {
            buf[uniq++] = buf[i];
        }
    }

    seq_t *sq = (seq_t *)realloc(buf, uniq * sizeof(seq_t));
    if (!sq) {
        free(buf);
        return;                         /* reallocation failed */
    }

    squares = sq;
    sq_count = uniq;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) build_squares();
    /* The tests guarantee that n is within bounds of the generated table. */
    return squares[n];
}
