/*
 * A115161  Numbers that are not the sum of a triangular number, a nonnegative
 *          cube and a fourth power.
 * https://oeis.org/A115161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:  a(n) = the nth positive integer that cannot be expressed as
 *                t + k³ + l⁴ where t = m(m+1)/2 (m ≥ 0), k³ ≥ 0, l⁴ ≥ 0.
 *                The algorithm precomputes all such representable sums up to a
 *                chosen bound, then extracts the missing numbers.  It rebuilds
 *                the list when a larger bound is required – this guarantees
 *                correctness for any n.
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* static storage for the sequence, updated lazily */
static seq_t *sequence = NULL;   /* array of non‑representable integers */
static size_t seq_len = 0;       /* how many have been stored */
static size_t seq_cap = 0;       /* current capacity of the array */
static idx_t   cur_bound = 0;    /* numeric limit used for the last build */

/* Helper: rebuild the sequence up to 'bound'.
 * The bound should be at least large enough to contain the requested n.
 */
static void build_seq(idx_t bound)
{
    /* reuse or allocate the mark array */
    unsigned char *mark = (unsigned char *)calloc(
            (size_t)(bound + 1), 1);
    if (!mark) exit(1);   /* abort on OOME */

    /* -------------------------------------------------------------
       Generate triangular numbers ≤ bound  ->  tri[]
    ------------------------------------------------------------ */
    idx_t *tri = NULL;
    size_t tri_cap = 0, tri_len = 0;
    for (idx_t m = 0; ; ++m) {
        idx_t t = (m * (m + 1)) / 2;
        if (t > bound) break;
        if (tri_len == tri_cap) {
            tri_cap = tri_cap ? tri_cap * 2 : 8;
            tri = (idx_t *)realloc(tri, tri_cap * sizeof(idx_t));
            if (!tri) exit(1);
        }
        tri[tri_len++] = t;
    }

    /* -------------------------------------------------------------
       Generate cubes ≤ bound  ->  cube[]
    ------------------------------------------------------------ */
    idx_t *cube = NULL;
    size_t cube_cap = 0, cube_len = 0;
    for (idx_t k = 0; ; ++k) {
        idx_t c = k * k * k;
        if (c > bound) break;
        if (cube_len == cube_cap) {
            cube_cap = cube_cap ? cube_cap * 2 : 8;
            cube = (idx_t *)realloc(cube, cube_cap * sizeof(idx_t));
            if (!cube) exit(1);
        }
        cube[cube_len++] = c;
    }

    /* -------------------------------------------------------------
       Generate fourth powers ≤ bound  ->  fourth[]
    ------------------------------------------------------------ */
    idx_t *fourth = NULL;
    size_t fourth_cap = 0, fourth_len = 0;
    for (idx_t l = 0; ; ++l) {
        idx_t f = l * l * l * l;
        if (f > bound) break;
        if (fourth_len == fourth_cap) {
            fourth_cap = fourth_cap ? fourth_cap * 2 : 8;
            fourth = (idx_t *)realloc(fourth, fourth_cap * sizeof(idx_t));
            if (!fourth) exit(1);
        }
        fourth[fourth_len++] = f;
    }

    /* -------------------------------------------------------------
       Mark every representable sum t + c + f
    ------------------------------------------------------------ */
    for (size_t i = 0; i < tri_len; ++i) {
        idx_t t = tri[i];
        for (size_t j = 0; j < cube_len; ++j) {
            idx_t c = cube[j];
            idx_t sum1 = t + c;
            if (sum1 > bound) continue;
            for (size_t k = 0; k < fourth_len; ++k) {
                idx_t f = fourth[k];
                idx_t s = sum1 + f;
                if (s <= bound) mark[s] = 1;
            }
        }
    }

    /* -------------------------------------------------------------
       Build the sequence array from the marks
    ------------------------------------------------------------ */
    /* free previous allocation, if any */
    if (sequence) {
        free(sequence);
        sequence = NULL;
        seq_len = 0;
        seq_cap = 0;
    }

    for (idx_t i = 1; i <= bound; ++i) {
        if (!mark[i]) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 8;
                sequence = (seq_t *)realloc(sequence, seq_cap * sizeof(seq_t));
                if (!sequence) exit(1);
            }
            sequence[seq_len++] = (seq_t)i;
        }
    }

    /* free temporary arrays */
    free(mark);
    free(tri);
    free(cube);
    free(fourth);
    cur_bound = bound;
}

/* -------------------------------------------------------------
   Public function required by the harness
 ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    /* Ensure we have an initial list large enough for typical use */
    if (sequence == NULL) {
        /* 1<<20 = 1,048,576 – contains > 10,000 terms. */
        build_seq(1 << 20);
    }

    /* If n exceeds current count, enlarge the bound and rebuild. */
    while (seq_len < (size_t)n) {
        idx_t newbound = cur_bound * 2;
        if (newbound == 0) newbound = 1 << 20; /* in case cur_bound was 0 */
        build_seq(newbound);
    }

    return sequence[n - 1];
}
