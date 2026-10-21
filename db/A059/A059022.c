/*
 * A059022  Triangle of Stirling numbers of order 3.
 * https://oeis.org/A059022
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf, nice
 * Formula:   a(n) evaluates the triangle of 3‑associated Stirling numbers
 *            S3(n,k). The triangle is built by the recurrence S3(n+1,k) =
 *            k*S3(n,k) + binomial(n,2)*S3(n-2,k-1) with initial values
 *            S3(n,1)=1 for all n≥3. Numbers are emitted row by row, skipping
 *            zeros, so that a(3)=S3(3,1), a(4)=S3(4,1), …. The fun...
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 56, u64 n <= 133,
 *            u128 n <= 341, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/binomial.h"   /* extern seq_t binomial(idx_t n, idx_t k); */

static seq_t **S3 = NULL;          /* S3[n][k] ; n ≥ 0 , k ≥ 0 */
static idx_t  S3_max_n = 0;        /* maximum n for which the triangle has been
                                     computed */

/* Build the triangle up to the requested row n (inclusive). */
static void ensure_s3_upto(idx_t n)
{
    if (n <= S3_max_n)
        return;                      /* already built */

    /* Grow the pointer array to hold rows 0..n. */
    size_t new_size = (size_t)(n + 1);
    seq_t **tmp = (seq_t **)realloc(S3, new_size * sizeof(seq_t *));
    if (tmp == NULL)
        abort();                     /* should never happen */
    S3 = tmp;

    /* Allocate and compute each new row. */
    for (idx_t i = S3_max_n + 1; i <= n; ++i)
    {
        size_t kmax = i / 3;         /* floor(i/3) */
        S3[i] = (seq_t *)calloc(kmax + 1, sizeof(seq_t));
        if (S3[i] == NULL)
            abort();                 /* should never happen */

        /* Base value: S3(i,1) = 1 for i ≥ 3. */
        if (i >= 3 && kmax >= 1)
            S3[i][1] = 1;

        /* Build k ≥ 2 entries using the recurrence. */
        for (size_t k = 2; k <= kmax; ++k)
        {
            seq_t part1 = 0, part2 = 0;

            /* k * S3(i-1, k) – only valid if k ≤ floor((i-1)/3). */
            if (k <= (i - 1) / 3)
                part1 = (seq_t)k * S3[i - 1][k];

            /* binomial(i-1,2) * S3(i-3, k-1) – only if i-3 ≥ 3 and
               k-1 ≤ floor((i-3)/3). */
            if (i >= 6 && (k - 1) <= (i - 3) / 3)
                part2 = binomial(i - 1, 2) * S3[i - 3][k - 1];

            S3[i][k] = part1 + part2;
        }
    }
    S3_max_n = n;
}

/* Convert the OEIS index n into the flattened triangle value. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                       /* undefined for n < 3 */

    /* First element (n=3) corresponds to S3(3,1).  Flatten target
       position within the non‑zero triangle entries. */
    idx_t target = n - 2;                /* 1‑based index in the flattened list */
    idx_t row = 3;

    while (1)
    {
        idx_t terms = row / 3;           /* number of entries in this row */
        if (target > terms)
        {
            target -= terms;
            ++row;
        }
        else
        {
            size_t col = target;         /* 1‑based column within the row */
            ensure_s3_upto(row);
            return S3[row][col];
        }
    }
}
