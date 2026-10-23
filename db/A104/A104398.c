/*
 * A104398  Sums of 8 distinct positive pentatope numbers (A000332).
 * https://oeis.org/A104398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of eight distinct positive pentatope numbers *
 *            Pentatope number P(k) = k*(k+1)*(k+2)*(k+3)/24
 * Range:     u8 none, u16 none, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *unique_sums = NULL; /* sorted unique sums */
static size_t unique_count = 0;   /* number of unique sums */
static bool   built = false;      /* has the table been built? */

/* Comparator for qsort (sequence values are unsigned integers) */
static int cmp_seq(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Build all sums of 8 distinct pentatope numbers, then sort and deduplicate */
static void build(void)
{
    if (built) return;

    /* choose an index limit that yields far more than 10 000 sums */
    const idx_t MAX_N = 25;                       /* 2 424 620 8‑combinations */

    /* pre‑compute pentatope numbers P[1..MAX_N] */
    seq_t *P = (seq_t *)malloc((MAX_N + 1) * sizeof(seq_t));
    if (!P) { built = true; return; }
    P[0] = 0;
    for (idx_t i = 1; i <= MAX_N; ++i) {
        seq_t a = (seq_t)i;
        seq_t prod = a * ((seq_t)(i + 1));
        prod *= ((seq_t)(i + 2));
        prod *= ((seq_t)(i + 3));
        P[i] = prod / (seq_t)24;        /* P(i) = i(i+1)(i+2)(i+3)/24 */
    }

    /* number of 8‑combinations of {1..MAX_N} */
    size_t combos = 1;
    for (idx_t k = 1; k <= 8; ++k)
        combos = combos * (MAX_N - k + 1) / k;

    seq_t *sums = (seq_t *)malloc(combos * sizeof(seq_t));
    if (!sums) { free(P); built = true; return; }
    size_t pos = 0;

    /* enumerate all 8‑combinations of indices and compute the sums */
    for (idx_t i1 = 1; i1 <= MAX_N - 7; ++i1)
    for (idx_t i2 = i1 + 1; i2 <= MAX_N - 6; ++i2)
    for (idx_t i3 = i2 + 1; i3 <= MAX_N - 5; ++i3)
    for (idx_t i4 = i3 + 1; i4 <= MAX_N - 4; ++i4)
    for (idx_t i5 = i4 + 1; i5 <= MAX_N - 3; ++i5)
    for (idx_t i6 = i5 + 1; i6 <= MAX_N - 2; ++i6)
    for (idx_t i7 = i6 + 1; i7 <= MAX_N - 1; ++i7)
    for (idx_t i8 = i7 + 1; i8 <= MAX_N; ++i8) {
        sums[pos++] = P[i1] + P[i2] + P[i3] + P[i4]
                    + P[i5] + P[i6] + P[i7] + P[i8];
    }

    /* sort the raw sums */
    qsort(sums, combos, sizeof(seq_t), cmp_seq);

    /* deduplicate in place, keeping only unique values */
    size_t uniq = 0;
    for (size_t i = 0; i < combos; ++i) {
        if (i == 0 || sums[i] != sums[i - 1])
            sums[uniq++] = sums[i];
    }

    /* shrink array to the number of unique sums */
    sums = (seq_t *)realloc(sums, uniq * sizeof(seq_t));
    unique_sums = sums;
    unique_count = uniq;

    built = true;
    free(P);
}

/* Sequence function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined offset 0 */
    build();                         /* ensure table is ready */
    if ((idx_t)(n - 1) >= unique_count) return 0; /* out of range */
    return unique_sums[n - 1];
}
