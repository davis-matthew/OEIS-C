/*
 * A058398  Partition triangle A008284 read from right to left.
 * https://oeis.org/A058398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, nice
 * Formula:   not stated
 * Range:     u8 n <= 343, u16 n <= 1815, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A058398
 *  a(n) is the flattening (row‑wise from left to right) of
 *  the partition triangle A008284, i.e.  a(n) = p(c-1, r-c+1)
 *  where r is the unique row with r*(r-1)/2 < n ≤ r*(r+1)/2
 *  and c = n - r*(r-1)/2 (1‑based column index).   This yields
 *  the first 24 terms
 *  1,1,1,1,1,1,1,1,2,1,1,1,2,2,1,1,1,2,3,3,1,1,1,2...
 *  The function builds the partition table p[n][m] for
 *  0 ≤ n,m ≤ MAX_ROW (MAX_ROW = 200 suffices for n ≤ 10⁴),
 *  then answers queries in O(√n) time and O(1) per call.
 */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build environment. */

#define MAX_ROW 200          /* sufficient for n ≤ 1e4 (r ≈ 141) */

static seq_t **p_table = NULL;
static bool p_initialized = false;

/* --------------------------------------------------------------------- */
/*  Build the partition table p[n][m] for 0 ≤ n,m ≤ MAX_ROW            */
static void init_partition_table(void)
{
    if (p_initialized) return;

    size_t i, m;
    p_table = (seq_t**)malloc((MAX_ROW + 1) * sizeof(seq_t*));
    for (i = 0; i <= MAX_ROW; ++i) {
        p_table[i] = (seq_t*)calloc((MAX_ROW + 1), sizeof(seq_t));
    }

    /* p(0,m) = 1 for all m */
    for (m = 0; m <= MAX_ROW; ++m) {
        p_table[0][m] = (seq_t)1;
    }

    /* p(n,0) = 0 for n > 0 */
    for (i = 1; i <= MAX_ROW; ++i) {
        p_table[i][0] = (seq_t)0;
    }

    /* recurrence: p(n,m) = p(n,m-1) + (n>=m ? p(n-m,m) : 0) */
    for (m = 1; m <= MAX_ROW; ++m) {
        for (i = 1; i <= MAX_ROW; ++i) {
            seq_t val = p_table[i][m-1];
            if (i >= m) {
                val = (seq_t)(val + p_table[i-m][m]);
            }
            p_table[i][m] = val;
        }
    }

    p_initialized = true;
}

/* --------------------------------------------------------------------- */
/*  Return the n-th term of A058398 (offset 1)                           */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for offset 1 */

    /* Initialize the partition table on first use */
    if (!p_initialized) init_partition_table();

    /* Find the row r such that r*(r-1)/2 < n ≤ r*(r+1)/2 */
    idx_t r = 1;
    idx_t base = 0;                /* sum of lengths of previous rows */
    while (base + r < n) {
        base += r;
        ++r;
    }

    /* Position within the row (0‑based) */
    idx_t pos = n - base - 1;       /* 0 ≤ pos < r */
    idx_t col = pos + 1;            /* 1‑based column index */

    /* Compute indices for the partition table:
     *      sub_n = col-1  (the "n" in p(n, m))
     *      sub_m = r-col+1 (the "m" in p(n, m))
     */
    idx_t sub_n = col - 1;
    idx_t sub_m = r - col + 1;

    /* Safety guard – enlarge table if necessary (rare) */
    if (sub_n > MAX_ROW || sub_m > MAX_ROW) {
        /* Recompute with a larger table – not expected for n ≤ 1e4 */
        size_t new_max = (sub_n > sub_m) ? sub_n : sub_m;
        size_t i, m;
        seq_t **new_table = (seq_t**)malloc((new_max + 1) * sizeof(seq_t*));
        for (i = 0; i <= new_max; ++i) {
            new_table[i] = (seq_t*)calloc((new_max + 1), sizeof(seq_t));
        }
        for (m = 0; m <= new_max; ++m) {
            new_table[0][m] = (seq_t)1;
        }
        for (i = 1; i <= new_max; ++i) {
            new_table[i][0] = (seq_t)0;
        }
        for (m = 1; m <= new_max; ++m) {
            for (i = 1; i <= new_max; ++i) {
                seq_t val = new_table[i][m-1];
                if (i >= m) {
                    val = (seq_t)(val + new_table[i-m][m]);
                }
                new_table[i][m] = val;
            }
        }
        /* Free old table */
        for (i = 0; i <= MAX_ROW; ++i) free(p_table[i]);
        free(p_table);
        p_table = new_table;
    }

    return p_table[sub_n][sub_m];
}
