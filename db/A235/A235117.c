/*
 * A235117  Irregular triangle read by rows: T(n,k) = number of independent
 *          vertex subsets of size k of the graph g_n obtained by attaching
 *          two pendant edges to each vertex of the ladder graph L_n (i.e.,
 *          L_n is the 2 X n grid graph; 0 <= k <= 4n+1).
 * https://oeis.org/A235117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened triangle of the independence polynomials
 *            of a family of graphs g_n, where the polynomials satisfy P(0) =
 *            1 P(1) = (1+4x+x^2)(1+x)^2 P(n) = (1+x)^2(1+3x+x^2) P(n-1) +
 *            x(1+x)^6 P(n-2) (n >= 2) The n‑th coefficient a(n) is read
 *            row‑by‑row from these polynomials. This implementa...
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <string.h>   /* memset */

#define MAX_R       2000                   /* sufficient for n ≈ 10 000   */
#define MAX_DEG    (4 * MAX_R + 1)         /* maximum polynomial length  */

/* buffers used for the recurrence; static to avoid reallocating every call */
static seq_t Pm2[MAX_DEG];                 /* P(k-2) */
static seq_t Pm1[MAX_DEG];                 /* P(k-1) */
static seq_t TMP[MAX_DEG];                 /* temporary storage */

/* polynomial multiplication helper */
static void conv(const seq_t *a, size_t len_a,
                 const seq_t *b, size_t len_b,
                 seq_t *dst, size_t dst_len)
{
    size_t i, j;
    for (i = 0; i < len_a; ++i)
    {
        if (a[i] == 0) continue;
        for (j = 0; j < len_b; ++j)
        {
            size_t pos = i + j;
            if (pos < dst_len)
                dst[pos] += a[i] * b[j];
        }
    }
}

seq_t a(idx_t n)
{
    /* ---------- locate the row r and index inside the row ---------- */
    idx_t r = 0;
    idx_t cum = 1;          /* T(0) = 1 entry */
    while (cum <= n)
    {
        ++r;
        cum = 2 * r * r + 3 * r + 1;
    }
    idx_t prev_cum = 2 * (r - 1) * (r - 1) + 3 * (r - 1) + 1;
    idx_t idx_in_row = n - prev_cum;   /* 0‑based within the row */

    /* ---------- trivial rows ---------- */
    if (r == 0)
        return (seq_t)1;
    if (r == 1)
    {
        static const seq_t row1[5] = {1, 6, 10, 6, 1};
        return row1[(size_t)idx_in_row];
    }

    /* ---------- initialize with P(0) and P(1) ---------- */
    {
        size_t i;
        for (i = 0; i < MAX_DEG; ++i)
            Pm2[i] = Pm1[i] = 0;
        Pm2[0] = 1;                                 /* P(0) */
        Pm1[0] = 1; Pm1[1] = 6; Pm1[2] = 10; Pm1[3] = 6; Pm1[4] = 1; /* P(1) */
    }

    /* ---------- polynomials used in the recurrence ---------- */
    static const seq_t POLY1[5] = {1, 5, 8, 5, 1};    /* (1+x)^2(1+3x+x^2) */
    static const seq_t POLY2[8] = {0, 1, 6, 15, 20, 15, 6, 1}; /* x(1+x)^6 */

    /* ---------- iterate the recurrence up to row r ---------- */
    for (size_t k = 2; k <= (size_t)r; ++k)
    {
        size_t len_k   = 4 * k + 1;          /* degree+1 of P(k)   */
        size_t len_km1 = 4 * (k - 1) + 1;    /* degree+1 of P(k-1) */
        size_t len_km2 = 4 * (k - 2) + 1;    /* degree+1 of P(k-2) */

        /* zero TEMP up to len_k */
        memset(TMP, 0, len_k * sizeof(seq_t));

        /* P(k-1) * POLY1 */
        conv(Pm1, len_km1, POLY1, 5, TMP, len_k);

        /* P(k-2) * POLY2 */
        conv(Pm2, len_km2, POLY2, 8, TMP, len_k);

        /* shift for next iteration: Pm2 <- Pm1, Pm1 <- P(k) */
        size_t i;
        for (i = 0; i < len_k; ++i)
        {
            Pm2[i] = Pm1[i];
            Pm1[i] = TMP[i];
        }
    }

    /* ---------- return requested coefficient ---------- */
    return Pm1[(size_t)idx_in_row];
}
