/*
 * A207877  Number of n X 6 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 0 and 1 1 1 vertically.
 * https://oeis.org/A207877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays avoiding horizontal patterns
 *            000 and 101 and vertical patterns 010 and 111. Computed by a
 *            transfer‑matrix dynamic programming
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 20, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#if defined(__cplusplus)
#  define ALLOC_SEQ_T_ARRAY(n) (new seq_t[(n)])
#  define FREE_SEQ_T_ARRAY(p) delete[] (p)
#else
#  define ALLOC_SEQ_T_ARRAY(n) ((seq_t*)calloc((n),sizeof(seq_t)))
#  define FREE_SEQ_T_ARRAY(p) free(p)
#endif

/* --------------------------------------------------------------------- */
/*  Static data for the transfer matrix.                                 */
/* --------------------------------------------------------------------- */
static unsigned          Ncol = 0;            /* number of valid column masks */
static uint8_t          *validMasks = NULL;   /* array of masks          */
static int               *maskToIdx  = NULL;   /* mapping mask -> index   */
static size_t            STATE_COUNT = 0;      /* Ncol * Ncol */

typedef struct { uint16_t *items; size_t len; } state_t;
static state_t          *succ = NULL;          /* transition lists       */

/* --------------------------------------------------------------------- */
/*  Build the list of column masks that satisfy the vertical constraints.*/
/* --------------------------------------------------------------------- */
static void init_masks(void)
{
    uint8_t   tmp[64];
    int       cnt = 0;

    for (int m = 0; m < 64; ++m) {
        bool ok = true;
        for (int i = 0; i <= 3; ++i) {
            int pat = (m >> i) & 7;          /* 3 consecutive bits */
            if (pat == 2 || pat == 7) {      /* 010 or 111 forbidden */
                ok = false;
                break;
            }
        }
        if (ok) {
            tmp[cnt++] = (uint8_t)m;
        }
    }

    Ncol = (unsigned)cnt;
    validMasks = (uint8_t*)malloc(Ncol * sizeof(uint8_t));
    memcpy(validMasks, tmp, Ncol * sizeof(uint8_t));

    maskToIdx = (int*)calloc(64, sizeof(int));
    for (unsigned i = 0; i < Ncol; ++i) {
        maskToIdx[validMasks[i]] = (int)i;
    }
}

/* --------------------------------------------------------------------- */
/*  Precompute the admissible transitions a->b->c for triples of masks.  */
/* --------------------------------------------------------------------- */
static void init_trans(void)
{
    STATE_COUNT = (size_t)Ncol * (size_t)Ncol;
    succ = (state_t*)calloc(STATE_COUNT, sizeof(*succ));

    /* First pass: count successors */
    size_t *cnts = (size_t*)calloc(STATE_COUNT, sizeof(size_t));
    for (unsigned a = 0; a < Ncol; ++a) {
        for (unsigned b = 0; b < Ncol; ++b) {
            size_t id_ab = (size_t)a * Ncol + b;
            for (unsigned c = 0; c < Ncol; ++c) {
                bool good = true;
                for (int r = 0; r < 6; ++r) {
                    int pat = ((validMasks[a] >> r) & 1) << 2 |
                              ((validMasks[b] >> r) & 1) << 1 |
                              ((validMasks[c] >> r) & 1);
                    if (pat == 0 || pat == 5) {        /* 000 or 101 */
                        good = false;
                        break;
                    }
                }
                if (good) cnts[id_ab]++;
            }
        }
    }

    /* Allocate successor lists */
    for (size_t s = 0; s < STATE_COUNT; ++s) {
        succ[s].len = cnts[s];
        if (cnts[s] > 0) {
            succ[s].items = (uint16_t*)malloc(cnts[s] * sizeof(uint16_t));
        }
    }

    /* Second pass: fill the lists */
    size_t *ptrs = (size_t*)calloc(STATE_COUNT, sizeof(size_t));
    for (unsigned a = 0; a < Ncol; ++a) {
        for (unsigned b = 0; b < Ncol; ++b) {
            size_t s = (size_t)a * Ncol + b;
            for (unsigned c = 0; c < Ncol; ++c) {
                bool good = true;
                for (int r = 0; r < 6; ++r) {
                    int pat = ((validMasks[a] >> r) & 1) << 2 |
                              ((validMasks[b] >> r) & 1) << 1 |
                              ((validMasks[c] >> r) & 1);
                    if (pat == 0 || pat == 5) {        /* 000 or 101 */
                        good = false;
                        break;
                    }
                }
                if (good) {
                    succ[s].items[ptrs[s]++] =
                        (uint16_t)((size_t)b * Ncol + c);
                }
            }
        }
    }

    free(cnts);
    free(ptrs);
}

/* --------------------------------------------------------------------- */
/*  Ensure that the static tables are built only once.                    */
/* --------------------------------------------------------------------- */
static void init_once(void)
{
    static bool initialized = false;
    if (!initialized) {
        init_masks();
        init_trans();
        initialized = true;
    }
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_once();

    /* First column: any valid mask */
    if (n == 1) {
        return (seq_t)Ncol;
    }

    /* DP arrays */
    size_t states = STATE_COUNT;
    seq_t *cur  = ALLOC_SEQ_T_ARRAY(states);
    seq_t *next = ALLOC_SEQ_T_ARRAY(states);
    if (!cur || !next) return (seq_t)0; /* allocation failure */

    /* Initialize for first two columns: all pair combinations are allowed */
    for (unsigned a = 0; a < Ncol; ++a) {
        for (unsigned b = 0; b < Ncol; ++b) {
            cur[(size_t)a * Ncol + b] = (seq_t)1;
        }
    }

    /* Iterate over remaining columns */
    for (idx_t col = 3; col <= n; ++col) {
        /* Clear next array */
        for (size_t i = 0; i < states; ++i) {
            next[i] = (seq_t)0;
        }

        for (size_t s = 0; s < states; ++s) {
            if (cur[s] == 0) continue;
            seq_t val = cur[s];
            for (size_t j = 0; j < succ[s].len; ++j) {
                next[succ[s].items[j]] += val;
            }
        }

        /* swap cur and next */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum over all ending states */
    seq_t ans = (seq_t)0;
    for (size_t s = 0; s < states; ++s) {
        ans += cur[s];
    }

    FREE_SEQ_T_ARRAY(cur);
    FREE_SEQ_T_ARRAY(next);
    return ans;
}
