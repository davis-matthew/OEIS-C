/*
 * A207964  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A207964: Number of 6×n binary arrays avoiding horizontally 000 and 101,
   and vertically 001 and 100.
   Computed by a transfer‑matrix DP with states = pairs of valid columns.
   Initial values: a(1)=22, a(2)=484.  */
#include <stdint.h>
#include <stdlib.h>

/* ---------- internal data -------------------------------------------- */
#define VLEN 22                 /* number of column patterns with no 001/100 */
#define STATECOUNT (VLEN*VLEN)  /* number of 2‑column states */

static uint8_t  colVals[VLEN];                   /* valid column bit patterns */
static uint16_t nextIds[STATECOUNT][VLEN];       /* adjacency lists of next state IDs */
static uint8_t  nextCnt[STATECOUNT];             /* number of successors for each state */

static seq_t *dp_cur = NULL;
static seq_t *dp_nxt = NULL;
static int    initialized = 0;

/* ---------- helper to build tables ----------------------------- */
static void build_tables(void)
{
    /* build list of valid columns */
    int vcnt = 0;
    for (int col = 0; col < 64; ++col) {
        int ok = 1;
        for (int i = 0; i <= 3; ++i) {         /* triples (i,i+1,i+2) */
            int tri = (col >> i) & 0b111;
            if (tri == 0b001 || tri == 0b100) { /* 001 or 100 prohibited */
                ok = 0;
                break;
            }
        }
        if (ok) {
            colVals[vcnt++] = (uint8_t)col;
        }
    }

    /* adjacency lists: from pair (a,b) to the new pair (b,c) */
    for (int a = 0; a < VLEN; ++a) {
        for (int b = 0; b < VLEN; ++b) {
            int id = a*VLEN + b;
            int ncnt = 0;
            for (int c = 0; c < VLEN; ++c) {
                int bip = 1;
                for (int r = 0; r < 6; ++r) { /* check all rows */
                    int x = (colVals[a] >> r) & 1;
                    int y = (colVals[b] >> r) & 1;
                    int z = (colVals[c] >> r) & 1;
                    int tri = (x<<2) | (y<<1) | z;
                    if (tri == 0 || tri == 5) { /* 000 or 101 forbidden */
                        bip = 0;
                        break;
                    }
                }
                if (bip) {
                    nextIds[id][ncnt++] = (uint16_t)(b*VLEN + c);
                }
            }
            nextCnt[id] = (uint8_t)ncnt;
        }
    }

    /* allocate DP arrays */
    dp_cur = (seq_t*)malloc(STATECOUNT * sizeof(seq_t));
    dp_nxt = (seq_t*)malloc(STATECOUNT * sizeof(seq_t));
    for (int i = 0; i < STATECOUNT; ++i) {
        dp_cur[i] = 0;
        dp_nxt[i] = 0;
    }
}

/* ---------- main function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not defined for offset 1 */

    /* ensure tables are built */
    if (!initialized) {
        build_tables();
        initialized = 1;
    }

    if (n == 1) return (seq_t)22;
    if (n == 2) return (seq_t)22 * (seq_t)22;

    /* initialise DP for arrays of length 2 (all states count=1) */
    for (int i = 0; i < STATECOUNT; ++i) dp_cur[i] = (seq_t)1;

    size_t iter = n - 2; /* number of transitions to reach length n */
    for (size_t step = 0; step < iter; ++step) {
        /* zero dp_nxt */
        for (int i = 0; i < STATECOUNT; ++i) dp_nxt[i] = 0;

        for (int state = 0; state < STATECOUNT; ++state) {
            seq_t val = dp_cur[state];
            if (val == 0) continue;
            int cnt = nextCnt[state];
            for (int k = 0; k < cnt; ++k) {
                int nxt = nextIds[state][k];
                dp_nxt[nxt] += val;
            }
        }
        /* swap */
        seq_t *tmp = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;
    }

    /* sum all states to get total number of arrays */
    seq_t result = 0;
    for (int i = 0; i < STATECOUNT; ++i) result += dp_cur[i];
    return result;
}
