/*
 * A207881  Number of nX4 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A207881 – Number of n×4 0–1 arrays avoiding
    horizontal 000 and 101 and vertical 011 and 110.
    The code implements a dynamic programming algorithm that
    enumerates admissible rows and stores a transition table
    between consecutive rows.  It works for any build that
    supplies the integer types seq_t and idx_t.
*/

#include <stdbool.h>

/* constant sizes – must be compile‑time constants (macros) */
#define MAX_ROWS   16          /* 4‑bit rows: 0 … 15 */
#define MAX_STATES (MAX_ROWS*MAX_ROWS)

/* tables built on first use */
static bool tables_ready = false;
static int  valid_rows[MAX_ROWS];
static int  num_valid_rows;
static int  next_len[MAX_STATES];
static int  next_list[MAX_STATES][MAX_ROWS];   /* at most MAX_ROWS per state */

/* ------------------------------------------------------------------- */
static void build_tables(void)
{
    if (tables_ready) return;

    /* ----- admissible rows (horizontal constraints) ----- */
    num_valid_rows = 0;
    for (int r = 0; r < MAX_ROWS; ++r) {
        bool ok = true;
        /* check the two horizontal triples in a 4‑bit row */
        for (int j = 0; j < 2; ++j) {
            int tri = (r >> j) & 7;           /* bits j, j+1, j+2 */
            if (tri == 0 || tri == 5) {       /* 000 or 101 */
                ok = false;
                break;
            }
        }
        if (ok) valid_rows[num_valid_rows++] = r;
    }

    /* ----- transition table (vertical constraints) ----- */
    for (int p2i = 0; p2i < num_valid_rows; ++p2i) {
        for (int p1i = 0; p1i < num_valid_rows; ++p1i) {
            int state = p2i * MAX_ROWS + p1i;   /* state index */
            int p2    = valid_rows[p2i];
            int p1    = valid_rows[p1i];
            int len   = 0;
            for (int curi = 0; curi < num_valid_rows; ++curi) {
                int cur = valid_rows[curi];
                bool ok = true;
                for (int j = 0; j < 4; ++j) {
                    int a = (p2 >> j) & 1;
                    int b = (p1 >> j) & 1;
                    int c = (cur >> j) & 1;
                    if ((a == 0 && b == 1 && c == 1) ||
                        (a == 1 && b == 1 && c == 0)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) next_list[state][len++] = curi;
            }
            next_len[state] = len;
        }
    }
    tables_ready = true;
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static arrays big enough for the maximum number of states */
    static seq_t dp1[MAX_STATES];
    static seq_t dp2[MAX_STATES];
    static seq_t *cur  = dp1;
    static seq_t *next = dp2;

    /* build tables once */
    build_tables();

    if (n == 0) return 0;          /* offset is 1 */

    /* trivial base cases */
    if (n == 1) return (seq_t)num_valid_rows;        /* a(1) = 9  */
    if (n == 2) return (seq_t)num_valid_rows * num_valid_rows; /* a(2) = 81 */

    /* Initialise dp for length = 2 : all pairs of admissible rows
       (vertical constraints are irrelevant for two rows)		 */
    for (int i = 0; i < MAX_STATES; ++i) cur[i] = 0;
    for (int i = 0; i < num_valid_rows; ++i)
        for (int j = 0; j < num_valid_rows; ++j)
            cur[i * MAX_ROWS + j] = 1;

    /* DP for rows 3 … n */
    for (idx_t step = 3; step <= n; ++step) {
        /* reset next array */
        for (int i = 0; i < MAX_STATES; ++i) next[i] = 0;

        /* transition */
        for (int state = 0; state < MAX_STATES; ++state) {
            seq_t cnt = cur[state];
            if (cnt == 0) continue;
            int p1i = state % MAX_ROWS;
            int len = next_len[state];
            for (int t = 0; t < len; ++t) {
                int curi = next_list[state][t];
                int new_state = p1i * MAX_ROWS + curi;
                next[new_state] += cnt;
            }
        }
        /* swap pointers for next iteration */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* Sum over all final state counts (only admissible pairs are non‑zero) */
    seq_t total = 0;
    for (int i = 0; i < num_valid_rows; ++i)
        for (int j = 0; j < num_valid_rows; ++j)
            total += cur[i * MAX_ROWS + j];

    return total;
}
