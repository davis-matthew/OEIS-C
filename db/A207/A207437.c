/*
 * A207437  Number of n X 3 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = number of n×3 binary arrays avoiding the
   horizontal patterns 001 and 010 and the vertical patterns
   001 and 100.  The function builds the values by
   dynamic programming on the last two rows.  */

#include <stdlib.h>
#include <string.h>

/* ---------- helper data ------------------------------------------- */
/* 3‑bit rows 0–7.  The rows 001 (1) and 010 (2) are forbidden. */
static const unsigned char valid_row[8] =
    {1,0,0,1,1,1,1,1};

/* ---------- global memoisation ------------------------------------- */
static seq_t *totals = NULL;          /* totals[n] = a(n), 1≤n≤computed */
static idx_t  totals_size = 0;        /* largest n for which totals is filled */

/* DP state: cur[p] is number of length‑cur_n lists ending with rows
   (p/8, p%8).  64 states 0…63. */
static seq_t cur[64] = {0}, next[64] = {0};

/* ---------- internal utility --------------------------------------- */
static void ensure_up_to(idx_t n)
{
    if (n <= totals_size) return;

    /* allocate or grow the totals array (size in bytes) */
    size_t bytes = (size_t)(n + 1) * sizeof(seq_t);
    seq_t *tmp = (seq_t *)realloc(totals, bytes);
    if (!tmp) abort();
    totals = tmp;

    /* initialise for n=1 and n=2 if nothing was computed yet */
    if (totals_size == 0) {
        totals[1] = (seq_t)6;            /* 6 admissible single rows */

        /* initialise cur for n=2: all admissible row pairs */
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                cur[i*8 + j] =
                    (seq_t)(valid_row[i] && valid_row[j] ? 1 : 0);
            }
        }
        /* sum over all pairs to get a(2) = 36 */
        seq_t sum = 0;
        for (int k = 0; k < 64; ++k) sum += cur[k];
        totals[2] = sum;
        totals_size = 2;
    }

    /* extend computation up to n */
    while (totals_size < n) {
        /* next = 0 */
        memset(next, 0, sizeof(next));

        /* Transition: from previous two rows (p2, p1) to (p1, r) */
        for (int p2 = 0; p2 < 8; ++p2) {
            if (!valid_row[p2]) continue;
            for (int p1 = 0; p1 < 8; ++p1) {
                if (!valid_row[p1]) continue;
                seq_t ways = cur[p2*8 + p1];
                if (!ways) continue;
                for (int r = 0; r < 8; ++r) {
                    if (!valid_row[r]) continue;

                    /* check vertical forbidden patterns on each column */
                    int ok = 1;
                    for (int col = 0; col < 3; ++col) {
                        int b2 = (p2 >> col) & 1;
                        int b1 = (p1 >> col) & 1;
                        int b0 = (r   >> col) & 1;
                        if ((b2==0 && b1==0 && b0==1) ||
                            (b2==1 && b1==0 && b0==0)) { ok = 0; break; }
                    }
                    if (!ok) continue;
                    int to = p1*8 + r;
                    next[to] += ways;
                }
            }
        }

        /* swap cur and next, and record the new total */
        memcpy(cur, next, sizeof(cur));
        seq_t total = 0;
        for (int k = 0; k < 64; ++k) total += cur[k];
        totals[++totals_size] = total;
    }
}

/* ---------- public interface --------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* defined only for n≥1 */
    if (n == 1) return (seq_t)6;   /* first term of the sequence */
    ensure_up_to(n);
    return totals[n];
}
