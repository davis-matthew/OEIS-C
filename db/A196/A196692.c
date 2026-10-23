/*
 * A196692  Number of nX4 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 2,1,3,0,4 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A196692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This sequence is defined by a constraint‑satisfaction * problem.
 *            The implementation uses dynamic programming over all * 4‑column
 *            patterns of values 0‑4 to enforce the neighbor rules
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PAT   625      /* 5^4 patterns */
#define MAX_MASK   16      /* 4‑bit mask for pending vertical requirements */
#define STATE_SIZ (MAX_PAT * MAX_MASK)

/* Precomputed tables for all patterns */
static int   pat_val[MAX_PAT][4];   /* value at each column of a pattern */
static int   horiz_cnt[MAX_PAT][4]; /* count of horizontal neighbors equal to
                                      target value for each cell */
static bool  pat_valid[MAX_PAT];    /* pattern satisfies horizontal constraints */

/* Target values for each cell value */
static const int target[5]       = {2,1,3,0,4};
static const int required_cnt[5] = {0,1,2,3,4};

/* Build tables once */
static void build_tables(void)
{
    int idx = 0;
    for (int a=0; a<5; ++a)
    for (int b=0; b<5; ++b)
    for (int c=0; c<5; ++c)
    for (int d=0; d<5; ++d) {
        pat_val[idx][0] = a;
        pat_val[idx][1] = b;
        pat_val[idx][2] = c;
        pat_val[idx][3] = d;
        /* horizontal counts */
        for (int col=0; col<4; ++col) {
            int val = pat_val[idx][col];
            int tgt = target[val];
            int cnt = 0;
            if (col>0   && pat_val[idx][col-1]==tgt) ++cnt;
            if (col<3   && pat_val[idx][col+1]==tgt) ++cnt;
            horiz_cnt[idx][col] = cnt;
        }
        /* pattern valid if no cell requires more horizontal matches than possible */
        bool ok = true;
        for (int col=0; col<4 && ok; ++col) {
            int val = pat_val[idx][col];
            if (horiz_cnt[idx][col] > required_cnt[val]) ok = false;
        }
        pat_valid[idx] = ok;
        ++idx;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n==0) return 0;
    static bool tables_built = false;
    if (!tables_built) { build_tables(); tables_built = true; }

    static seq_t dp0[STATE_SIZ];
    static seq_t dp1[STATE_SIZ];
    static seq_t *dp_cur = dp0;
    static seq_t *dp_next = dp1;

    static int active[STATE_SIZ];
    static int new_active[STATE_SIZ];
    int active_cnt = 0, new_active_cnt = 0;

    /* initialise first row */
    memset(dp_cur, 0, sizeof(dp_cur[0])*STATE_SIZ);
    for (int pat=0; pat<MAX_PAT; ++pat) {
        if (!pat_valid[pat]) continue;
        int mask = 0;
        bool ok = true;
        for (int col=0; col<4; ++col) {
            int val = pat_val[pat][col];
            int rem = required_cnt[val] - horiz_cnt[pat][col];
            if (rem<0 || rem>1) { ok=false; break; }
            if (rem==1) mask |= (1<<col);
        }
        if (!ok) continue;
        int state = pat*MAX_MASK + mask;
        dp_cur[state] = 1;
        active[active_cnt++] = state;
    }

    /* process subsequent rows */
    for (idx_t row=2; row<=n; ++row) {
        memset(dp_next, 0, sizeof(dp_next[0])*STATE_SIZ);
        new_active_cnt = 0;

        for (int ai=0; ai<active_cnt; ++ai) {
            int state = active[ai];
            int pat   = state >> 4;      /* /MAX_MASK */
            int mask_pat = state & (MAX_MASK-1); /* %MAX_MASK */
            seq_t ways = dp_cur[state];
            if (ways==0) continue;

            /* precompute allowed values for each column in the next pattern */
            int allowed[4][5];
            int width[4];
            for (int col=0; col<4; ++col) {
                int tgt = target[pat_val[pat][col]];
                if (mask_pat & (1<<col)) {
                    allowed[col][0] = tgt;
                    width[col] = 1;
                } else {
                    int k=0;
                    for (int v=0; v<5; ++v)
                        if (v!=tgt) { allowed[col][k++] = v; }
                    width[col] = 4;
                }
            }

            /* enumerate all compatible next patterns */
            for (int a0=0; a0<width[0]; ++a0)
            for (int a1=0; a1<width[1]; ++a1)
            for (int a2=0; a2<width[2]; ++a2)
            for (int a3=0; a3<width[3]; ++a3) {
                int npat = (((allowed[0][a0]*5)+allowed[1][a1])*5+allowed[2][a2])*5+allowed[3][a3];
                if (!pat_valid[npat]) continue;
                int mask_npat = 0;
                bool ok = true;
                for (int col=0; col<4 && ok; ++col) {
                    int val = pat_val[npat][col];
                    int up  = (pat_val[pat][col]==target[val]) ? 1 : 0;
                    int rem = required_cnt[val] - horiz_cnt[npat][col] - up;
                    if (rem<0 || rem>1) { ok=false; break; }
                    if (rem==1) mask_npat |= (1<<col);
                }
                if (!ok) continue;
                int new_state = npat*MAX_MASK + mask_npat;
                seq_t prev = dp_next[new_state];
                dp_next[new_state] = prev + ways;
                if (prev==0) new_active[new_active_cnt++] = new_state;
            }
        }

        /* swap dp arrays and active lists */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
        active_cnt = new_active_cnt;
        for (int i=0;i<active_cnt;++i) active[i] = new_active[i];
    }

    /* sum states with no pending vertical requirements (mask == 0) */
    seq_t result = 0;
    for (int ai=0; ai<active_cnt; ++ai) {
        int state = active[ai];
        if ((state & (MAX_MASK-1))==0) result += dp_cur[state];
    }
    return result;
}
