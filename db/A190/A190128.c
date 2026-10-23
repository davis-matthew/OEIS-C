/*
 * A190128  Numbers 1 through 10000 sorted lexicographically in ternary
 *          representation.
 * https://oeis.org/A190128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the n-th element of the numbers from 1 to 10000 sorted
 *            lexicographically by their base‑3 representation (1‑based index)
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Comparer used by qsort: lexicographic comparison of the base‑3 strings
   of the two numbers.  The numbers are small (≤ 10000), so a simple
   conversion to uint64_t is safe even if seq_t is an arbitrary‑precision
   type with a conversion operator to unsigned long long. */
static int lex_cmp(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    seq_t va = *pa;
    seq_t vb = *pb;
    if (va == vb) return 0;

    /* convert to a plain integer type for extracting base‑3 digits */
    uint64_t x = (uint64_t)va;
    uint64_t y = (uint64_t)vb;

    /* collect digits in reverse order (least significant first) */
    char sx[9], sy[9];
    int lx = 0, ly = 0;
    do { sx[lx++] = (char)('0' + (x % 3)); } while (x /= 3);
    do { sy[ly++] = (char)('0' + (y % 3)); } while (y /= 3);

    /* reverse to most significant first */
    for (int i = 0, j = lx - 1; i < j; ++i, --j) {
        char t = sx[i]; sx[i] = sx[j]; sx[j] = t;
    }
    for (int i = 0, j = ly - 1; i < j; ++i, --j) {
        char t = sy[i]; sy[i] = sy[j]; sy[j] = t;
    }

    /* lexicographic comparison */
    int mn = lx < ly ? lx : ly;
    for (int i = 0; i < mn; ++i) {
        if (sx[i] != sy[i]) return sx[i] < sy[i] ? -1 : 1;
    }
    if (lx < ly) return -1;
    if (lx > ly) return 1;
    return 0;
}

/* Static storage for the sorted list (initialized lazily). */
static seq_t *sorted = NULL;
static bool init_done = false;

seq_t a(idx_t n)
{
    if (!init_done) {
#if defined(__cplusplus)
        sorted = new seq_t[10000];
#else
        sorted = (seq_t*)malloc(sizeof(seq_t) * 10000);
#endif
        for (idx_t i = 0; i < 10000; ++i) sorted[i] = (seq_t)(i + 1);
        qsort(sorted, 10000, sizeof(seq_t), lex_cmp);
        init_done = true;
    }
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */
    if (n > 10000) return 0;      /* out of range – caller should avoid */
    return sorted[(idx_t)(n - 1)];
}
