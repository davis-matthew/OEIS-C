/*
 * A235960  Number of (n+1)X(6+1) 0..1 arrays with the difference of the upper
 *          and lower median value of each 2X2 subblock in lexicographically
 *          nondecreasing order columnwise and nonincreasing rowwise.
 * https://oeis.org/A235960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A235960 – Number of (n+1)×7 binary arrays where,
    for each adjacent pair of rows, the vector of “difference”
    values of all 2×2 subblocks is nondecreasing from left to right,
    and these vectors are nonincreasing when read from top to bottom.
    The implementation enumerates rows as 7‑bit numbers and uses
    dynamic programming over the maximal prefix‑length λ of 1‑bits
    seen so far.  λ∈{0,…,6} because a vector of 6 columns can have
    at most 6 ones in a prefix of 1‑bits.  For a fixed first row
    any λ is allowed; each subsequent row may only use a λ
    that is ≤ the previous λ.  The program builds an adjacency
    list of compatible row pairs for each λ once, then counts
    all (n+1)×7 arrays by iterating n transitions. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------------------------------------------------------------------- */
/*  Constants describing the search space.  These are compile‑time
    constants to satisfy the C11/ C++17 requirements for array bounds.  */
#define ROWS          (1 << 7)   /* 7 columns → 128 possible rows       */
#define MAXLAMBDA     6          /* λ ∈ {0,…,6}                          */
#define MAXNEIGH      128        /* maximum successors per (λ,row)       */

/* ---------------------------------------------------------------------- */
/*  Pre‑computed adjacency lists.  Each entry neighbors[λ][r] holds
    the rows that can follow row r while the λ computed from the
    2×2 blocks equals the given λ.  The list length for each
    (λ,r) pair is stored in neigh_len[λ][r].                    */
static unsigned char neigh_len[MAXLAMBDA + 1][ROWS];
static unsigned char neighbors[MAXLAMBDA + 1][ROWS][MAXNEIGH];
static bool neighbors_built = false;

/* ---------------------------------------------------------------------- */
/*  Build the adjacency lists on first invocation.                       */
static void build_neighbors(void)
{
    if (neighbors_built) return;

    /* Initialise lengths to zero. */
    for (int lam = 0; lam <= MAXLAMBDA; ++lam)
        for (int r = 0; r < ROWS; ++r)
            neigh_len[lam][r] = 0;

    /* Enumerate all ordered row pairs. */
    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int r2 = 0; r2 < ROWS; ++r2) {
            unsigned mask = 0;
            /* Compute mask of columns where the 2×2 block has 2 ones. */
            for (int j = 0; j < 6; ++j) {
                int a1 = (r1 >>  j)     & 1;
                int a2 = (r1 >> (j+1)) & 1;
                int b1 = (r2 >>  j)     & 1;
                int b2 = (r2 >> (j+1)) & 1;
                int sum = a1 + a2 + b1 + b2;
                if (sum == 2) mask |= (unsigned)(1u << j);
            }

            /* Determine λ such that mask == (1<<λ)-1. */
            int lam = -1;
            for (int l = 0; l <= MAXLAMBDA; ++l)
                if (mask == ((1u << l) - 1u)) { lam = l; break; }
            if (lam < 0) continue;  /* no valid λ, skip */

            /* Record transition r1 → r2 under λ. */
            unsigned char idx = neigh_len[lam][r1];
            if (idx < MAXNEIGH)
                neighbors[lam][r1][idx] = (unsigned char)r2;
            neigh_len[lam][r1] = idx + 1;
        }
    }
    neighbors_built = true;
}

/* ---------------------------------------------------------------------- */
/*  Dynamic‑programming routine.  dp_cur[r][m] counts the number of
    arrays where the last row is r and the largest λ seen so far
    (i.e. the column‑wise non‑decreasing constraint) is m. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1 */

    build_neighbors();

    static seq_t dp1[ROWS][MAXLAMBDA + 1];
    static seq_t dp2[ROWS][MAXLAMBDA + 1];

    seq_t (*cur)[MAXLAMBDA + 1] = dp1;
    seq_t (*next)[MAXLAMBDA + 1] = dp2;

    /* Initialise: any first row is allowed; the current λ limit is 6. */
    for (int r = 0; r < ROWS; ++r)
        for (int m = 0; m <= MAXLAMBDA; ++m)
            cur[r][m] = 0;
    for (int r = 0; r < ROWS; ++r)
        cur[r][MAXLAMBDA] = 1;

    /* Perform n transitions (i.e., n steps between rows). */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero out the next table. */
        for (int r = 0; r < ROWS; ++r)
            for (int m = 0; m <= MAXLAMBDA; ++m)
                next[r][m] = 0;

        for (int r = 0; r < ROWS; ++r) {
            for (int m = 0; m <= MAXLAMBDA; ++m) {
                seq_t ways = cur[r][m];
                if (ways == (seq_t)0) continue;

                /* Try all λ values allowed by the current limit m. */
                for (int lam = 0; lam <= m; ++lam) {
                    unsigned char *succ = neighbors[lam][r];
                    unsigned char cnt   = neigh_len[lam][r];
                    for (unsigned char k = 0; k < cnt; ++k) {
                        int r2 = succ[k];
                        next[r2][lam] += ways;
                    }
                }
            }
        }

        /* Swap current and next tables for next iteration. */
        seq_t (*tmp)[MAXLAMBDA + 1] = cur;
        cur = next; next = tmp;
    }

    /* Sum all possibilities over all ending rows and λ limits. */
    seq_t total = (seq_t)0;
    for (int r = 0; r < ROWS; ++r)
        for (int m = 0; m <= MAXLAMBDA; ++m)
            total += cur[r][m];

    return total;
}
