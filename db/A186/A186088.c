/*
 * A186088  Number of (n+2)X3 0..4 arrays with each 3X3 subblock having rows
 *          and columns in lexicographically nondecreasing order.
 * https://oeis.org/A186088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by building a DP over pairs of rows. The first
 *            35 values are pre‑computed and the remaining values are obtained
 *            by a Newton series: a(n) = Σ_{k=0}^{34} γ_k * C(n-1,k) for n>35.
 *            The γ_k are the forward differences of the first 35 terms
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 26, u128 n <= 147,
 *            big any n
 * Helpers:   row_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/row_le.h"

#define ROWS 125                  /* 5^3 possible rows */
#define PAIRS (ROWS * ROWS)       /* pairs of consecutive rows */

/* Row representations in base‑5 */
static uint8_t row_d[ROWS][3];

/* Transition tables */
static uint8_t  *trans_cnt;      /* number of outgoing edges per state */
static size_t   *trans_start;    /* start index in trans_next for each state */
static uint16_t *trans_next;     /* concatenated destinations */

/* DP buffers */
static seq_t dp0[PAIRS];
static seq_t dp1[PAIRS];

/* Pre‑computed values and Newton coefficients */
static seq_t a_vals[36];      /* a_vals[1] … a_vals[35] */
static seq_t gamma[35];       /* γ_0 … γ_34 */

/* One‑time initialisation flag */
static bool init_done = false;

/* --------------------------------------------------------------------
   Initialise row_d with all 5‑ary 3‑vectors
--------------------------------------------------------------------*/
static void init_rows(void)
{
    for (int i = 0; i < ROWS; ++i)
    {
        row_d[i][0] = (i / 25) % 5;   /* most significant digit */
        row_d[i][1] = (i / 5) % 5;
        row_d[i][2] = i % 5;          /* least significant digit */
    }
}

/* --------------------------------------------------------------------
   Build transition tables
--------------------------------------------------------------------*/
static void build_transitions(void)
{
    size_t i, j, r, idx;
    size_t total = 0;

    trans_cnt   = (uint8_t *)malloc(PAIRS * sizeof(uint8_t));
    trans_start = (size_t  *)malloc((PAIRS + 1) * sizeof(size_t));

    for (i = 0; i < ROWS; ++i)
    {
        for (j = 0; j < ROWS; ++j)
        {
            idx = i * ROWS + j;
            uint8_t cnt = 0;

            if (!row_le(row_d[i], row_d[j]))
            {
                trans_cnt[idx] = 0;
                continue;
            }

            for (r = 0; r < ROWS; ++r)
            {
                if (!row_le(row_d[j], row_d[r]))
                    continue;

                const uint8_t *A = row_d[i];
                const uint8_t *B = row_d[j];
                const uint8_t *C = row_d[r];

                bool ok = false;
                /* column 0 <= column 1 */
                if (A[0] < A[1]) ok = true;
                else if (A[0] == A[1])
                {
                    if (B[0] < B[1]) ok = true;
                    else if (B[0] == B[1])
                        ok = (C[0] <= C[1]);
                }
                if (!ok) continue;

                ok = false;
                /* column 1 <= column 2 */
                if (A[1] < A[2]) ok = true;
                else if (A[1] == A[2])
                {
                    if (B[1] < B[2]) ok = true;
                    else if (B[1] == B[2])
                        ok = (C[1] <= C[2]);
                }
                if (!ok) continue;

                ++cnt;
            }
            trans_cnt[idx] = cnt;
            total += cnt;
        }
    }

    trans_start[0] = 0;
    for (i = 0; i < PAIRS; ++i)
        trans_start[i + 1] = trans_start[i] + trans_cnt[i];

    trans_next = (uint16_t *)malloc(total * sizeof(uint16_t));

    for (i = 0; i < ROWS; ++i)
    {
        for (j = 0; j < ROWS; ++j)
        {
            idx = i * ROWS + j;
            size_t pos = trans_start[idx];

            if (!row_le(row_d[i], row_d[j]))
                continue;

            for (r = 0; r < ROWS; ++r)
            {
                if (!row_le(row_d[j], row_d[r]))
                    continue;

                const uint8_t *A = row_d[i];
                const uint8_t *B = row_d[j];
                const uint8_t *C = row_d[r];

                bool ok = false;
                if (A[0] < A[1]) ok = true;
                else if (A[0] == A[1])
                {
                    if (B[0] < B[1]) ok = true;
                    else if (B[0] == B[1])
                        ok = (C[0] <= C[1]);
                }
                if (!ok) continue;

                ok = false;
                if (A[1] < A[2]) ok = true;
                else if (A[1] == A[2])
                {
                    if (B[1] < B[2]) ok = true;
                    else if (B[1] == B[2])
                        ok = (C[1] <= C[2]);
                }
                if (!ok) continue;

                trans_next[pos++] = (uint16_t)(j * ROWS + r);
            }
        }
    }
}

/* --------------------------------------------------------------------
   Pre‑compute the first 35 terms via DP
--------------------------------------------------------------------*/
static void precompute_first_terms(void)
{
    init_rows();
    build_transitions();

    for (size_t i = 0; i < PAIRS; ++i)
        dp0[i] = 1;

    seq_t *cur  = dp0;
    seq_t *next = dp1;

    for (idx_t step = 1; step <= 35; ++step)
    {
        for (size_t i = 0; i < PAIRS; ++i)
            next[i] = 0;

        for (size_t s = 0; s < PAIRS; ++s)
        {
            seq_t val = cur[s];
            if (!val) continue;

            size_t st = trans_start[s];
            size_t en = trans_start[s + 1];
            for (size_t p = st; p < en; ++p)
                next[trans_next[p]] += val;
        }

        seq_t total = 0;
        for (size_t i = 0; i < PAIRS; ++i)
            total += next[i];
        a_vals[step] = total;

        seq_t *tmp = cur;
        cur         = next;
        next        = tmp;
    }
}

/* --------------------------------------------------------------------
   Compute Newton series coefficients γ_k = Δ^k a(1)
--------------------------------------------------------------------*/
static void compute_gamma(void)
{
    gamma[0] = a_vals[1];
    seq_t diff[36];               /* indices 1..35 used */
    for (idx_t i = 1; i <= 35; ++i)
        diff[i] = a_vals[i];

    for (idx_t k = 1; k <= 34; ++k)
    {
        for (idx_t i = 1; i <= 35 - k; ++i)
            diff[i] = diff[i + 1] - diff[i];
        gamma[k] = diff[1];
    }
}

/* --------------------------------------------------------------------
   One‑time initialisation
--------------------------------------------------------------------*/
static void init_sequence(void)
{
    precompute_first_terms();
    compute_gamma();
    init_done = true;
}

/* --------------------------------------------------------------------
   Main sequence function
--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (!init_done)
        init_sequence();

    if (n <= 35)
        return a_vals[n];

    seq_t ans   = gamma[0];
    seq_t binom = 1;                /* C(n-1,0) */

    for (idx_t k = 1; k <= 34; ++k)
    {
        binom = binom * ((seq_t)(n - k));
        binom = binom / (seq_t)k;  /* exact division */

        ans += gamma[k] * binom;
    }
    return ans;
}
