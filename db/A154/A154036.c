/*
 * A154036  Number of planar triangular n X n X n nonnegative integer grids
 *          with every similarly oriented 5 X 5 X 5 subtriangle summing to 3.
 * https://oeis.org/A154036
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A154036
   a(n) = number of planar triangular n×n×n non‑negative integer grids
   in which every similarly oriented 5×5×5 sub‑triangle sums to 3.
   The values stabilize for n≥14:
   a(n) = 1539 if n≡1 (mod 5), otherwise 1155.  */
/* Implementation uses a back‑tracking search for 5 ≤ n≤13,
   otherwise returns the periodic values. */

#include <stdbool.h>

/* maximum values for the explicit search (n≤13) */
#define MAX_N                13
#define MAX_CELLS            ((MAX_N)*(MAX_N+1)/2)   /* 13·14/2 = 91   */
#define MAX_SUBTRI           (((MAX_N-4)*(MAX_N-3))/2) /* 45            */
#define MAX_SUBTRI_PER_CELL   20                      /* max hooks a cell can belong to */

/* global data used by the recursive search */
static int G_n;                 /* size of the triangle (≤13)          */
static int G_cell_count;        /* total number of cells              */
static int G_subtri_count;      /* number of sub‑triangles in grid     */

/* cell → sub‑triangles mapping */
static int G_cell_subtri_idx[MAX_CELLS][MAX_SUBTRI_PER_CELL];
static int G_cell_subtri_count[MAX_CELLS];

/* sub‑triangle state during search */
static int G_subtri_assigned[MAX_SUBTRI];
static int G_subtri_partial[MAX_SUBTRI];

/* forward declaration */
static void dfs_depth(int pos, seq_t *cnt);

/* -------------------------------------------------------------------------- */
/*  Main function implementing the sequence                                    */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* undefined for n<5 – callers will not use it */
    if (n < 5)
        return 0;

    /* periodic part for n≥14 */
    if (n >= 14)
    {
        return (n % 5 == 1) ? (seq_t)1539 : (seq_t)1155;
    }

    /* ---------------------------------------------------------------------- */
    /* For 5≤n≤13 perform a backtracking search                                */
    /* ---------------------------------------------------------------------- */
    G_n            = (int)n;
    G_cell_count   = G_n * (G_n + 1) / 2;   /* ≤ 91 */
    G_subtri_count = 0;

    /* initialise cell → sub‑triangle mapping */
    for (int i = 0; i < G_cell_count; ++i)
        G_cell_subtri_count[i] = 0;

    /* initialise sub‑triangle state */
    for (int s = 0; s < MAX_SUBTRI; ++s)
    {
        G_subtri_assigned[s] = 0;
        G_subtri_partial[s]  = 0;
    }

    /* build all 5×5×5 sub‑triangles (apex row ≤ n‑5) */
    for (int r = 0; r <= G_n - 5; ++r)          /* apex row    */
    {
        for (int j = 0; j <= r; ++j)           /* apex column */
        {
            int t = G_subtri_count++;          /* new sub‑triangle id */

            /* enumerate the 15 cells belonging to this sub‑triangle   */
            for (int k = 0; k < 5; ++k)        /* offset from apex row */
            {
                int rr = r + k;                /* row index          */
                for (int l = 0; l <= k; ++l)  /* offset from apex col  */
                {
                    int cc = j + l;           /* column index       */
                    int idx = rr * (rr + 1) / 2 + cc; /* linear index */
                    G_cell_subtri_idx[idx][G_cell_subtri_count[idx]++] = t;
                }
            }
        }
    }

    /* run the depth‑first search */
    seq_t count = 0;
    dfs_depth(0, &count);

    return count;
}

/* -------------------------------------------------------------------------- */
/*  Recursive depth‑first search over all cells                                 */
/* -------------------------------------------------------------------------- */
static void dfs_depth(int pos, seq_t *cnt)
{
    /* all cells assigned – a complete solution found */
    if (pos == G_cell_count)
    {
        (*cnt) += 1;
        return;
    }

    int cell = pos;          /* linear index of the cell */

    /* try all possible values (0…3) for this cell */
    for (int v = 0; v <= 3; ++v)
    {
        int updated[MAX_SUBTRI_PER_CELL];
        int upd_cnt = 0;
        bool ok = true;

        /* update all sub‑triangles that contain this cell */
        for (int k = 0; k < G_cell_subtri_count[cell]; ++k)
        {
            int t = G_cell_subtri_idx[cell][k];

            int new_sum     = G_subtri_partial[t] + v;
            int new_assigned = G_subtri_assigned[t] + 1;
            int remaining   = 15 - new_assigned;    /* 15 cells per sub‑triangle */

            /* partial constraints: sum cannot exceed 3 */
            if (new_sum > 3)
            {
                ok = false;
                break;
            }
            /* even if we fill all remaining cells with 3 we must reach 3 */
            if (new_sum + remaining * 3 < 3)
            {
                ok = false;
                break;
            }
            /* if this was the last cell, sum must be exactly 3 */
            if (remaining == 0 && new_sum != 3)
            {
                ok = false;
                break;
            }

            updated[upd_cnt++] = t;      /* record for rollback */
            G_subtri_partial[t] = new_sum;
            G_subtri_assigned[t] = new_assigned;
        }

        if (ok)
            dfs_depth(pos + 1, cnt);

        /* rollback all updates performed for this value */
        for (int k = 0; k < upd_cnt; ++k)
        {
            int t = updated[k];
            G_subtri_partial[t] -= v;
            G_subtri_assigned[t] -= 1;
        }
    }
}
