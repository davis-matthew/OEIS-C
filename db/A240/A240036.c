/*
 * A240036  Number of nX5 0..2 arrays with no element equal to zero plus the
 *          sum of elements to its left or zero plus the sum of elements above
 *          it or zero plus the sum of the elements diagonally to its
 *          northwest, modulo 3.
 * https://oeis.org/A240036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 34, u64 n <= 70, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A240036 – Number of n×5 0..2 arrays with
    the property that each element is not equal to the sum of all
    elements to its left, to its above, or to its north‑west
    diagonal, taken modulo 3.  This implementation
    uses a state–transition automaton with 3^10=59049 states
    and computes the sequence by dynamic programming.  */

#include <stdlib.h>
#include <stdint.h>

#define STATES 59049          /* 3^10 */

typedef struct {
    int next[32];            /* destinations */
    unsigned char sz;        /* number of transitions (≤32) */
} Trans;

/* Global static data – initialised on first call to a() */
static seq_t *cur = NULL;          /* current state counts */
static seq_t *next = NULL;         /* next state counts */
static Trans *transitions = NULL;  /* transition tables */
static seq_t *seqarr = NULL;       /* computed values a(1)...a(k) */
static size_t seq_len = 1;         /* number of stored values (seqarr[0] unused) */
static size_t seq_cap = 0;         /* capacity of seqarr */
static int initialized = 0;

/* ------------------------------------------------------------------ */
/* Helper: encode a state consisting of 10 base‑3 digits
   (5 column sums + 5 diagonal sums).                                 */
static inline int encode_state(const int col[5], const int diag[5])
{
    int idx = 0, pow = 1;
    for (int i = 0; i < 5; ++i) {
        idx += col[i] * pow;
        pow *= 3;
    }
    for (int i = 0; i < 5; ++i) {
        idx += diag[i] * pow;
        pow *= 3;
    }
    return idx;
}

/* Helper: decode a state index to 10 base‑3 digits. */
static inline void decode_state(int idx, int col[5], int diag[5])
{
    int v = idx;
    for (int i = 0; i < 5; ++i) {
        col[i] = v % 3;
        v /= 3;
    }
    for (int i = 0; i < 5; ++i) {
        diag[i] = v % 3;
        v /= 3;
    }
}

/* ------------------------------------------------------------------ */
/* Recursive generator of all valid rows for a given state.
   For each completed row the destination state is appended to the
   transition table 't'.                                          */
static void build_dfs(int pos, int left,
                      int cur_col[5], int row_contrib[5],
                      const int diag[5], Trans *t)
{
    if (pos == 5) {
        int new_diag[5];
        for (int k = 0; k < 4; ++k)
            new_diag[k] = (diag[k + 1] + row_contrib[k + 1]) % 3;
        new_diag[4] = 0;          /* newmost diagonal has no past cells */
        int next_state = encode_state(cur_col, new_diag);
        if (t->sz < 32)             /* safety: should never exceed 32 */
            t->next[t->sz++] = next_state;
        return;
    }

    int j = pos;                     /* column index 0..4 */
    int old_colj = cur_col[j];
    int forbid1 = left;              /* sum of left cells (mod 3) */
    int forbid2 = old_colj;          /* current column sum (mod 3) */
    int forbid3 = diag[4 - j];       /* diagonal sum of offset i-j (mod 3) */

    for (int val = 0; val < 3; ++val) {
        if (val == forbid1 || val == forbid2 || val == forbid3)
            continue;

        cur_col[j] = (old_colj + val) % 3;
        row_contrib[4 - j] = val;
        build_dfs(pos + 1, (left + val) % 3, cur_col, row_contrib, diag, t);
        cur_col[j] = old_colj;       /* backtrack */
    }
}

/* ------------------------------------------------------------------ */
/* Build the transition tables for all states. */
static void build_transitions(void)
{
    transitions = (Trans *)calloc(STATES, sizeof(Trans));
    int col[5], diag[5], cur_col[5], row_contrib[5];

    for (int idx = 0; idx < STATES; ++idx) {
        decode_state(idx, col, diag);
        memcpy(cur_col, col, sizeof(col));
        memset(row_contrib, 0, sizeof(row_contrib));
        Trans *t = &transitions[idx];
        t->sz = 0;
        build_dfs(0, 0, cur_col, row_contrib, diag, t);
    }
}

/* ------------------------------------------------------------------ */
/* Ensure the global arrays are initialised. */
static void ensure_init(void)
{
    if (initialized)
        return;
    initialized = 1;

    /* allocate DP state arrays */
    cur   = (seq_t *)calloc(STATES, sizeof(seq_t));
    next  = (seq_t *)calloc(STATES, sizeof(seq_t));

    /* initial state: all sums 0, count 1 */
    cur[0] = 1;

    /* allocate sequence storage */
    seq_cap = 64;                    /* initial capacity */
    seqarr = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
    seqarr[0] = 0;                   /* unused for offset 0 */

    /* build state transition tables once */
    build_transitions();
}

/* ------------------------------------------------------------------ */
/* Extend the sequence up to at least index n. */
static void extend_sequence(idx_t n)
{
    ensure_init();

    while (seq_len <= n) {
        /* zero 'next' array */
        for (int i = 0; i < STATES; ++i)
            next[i] = 0;

        /* propagate counts through the transition graph */
        for (int s = 0; s < STATES; ++s) {
            seq_t val = cur[s];
            if (val == 0)
                continue;
            Trans *t = &transitions[s];
            for (int k = 0; k < t->sz; ++k) {
                int ns = t->next[k];
                next[ns] += val;
            }
        }

        /* compute total number of arrays for this size */
        seq_t total = 0;
        for (int i = 0; i < STATES; ++i)
            total += next[i];

        /* store the new term */
        if (seq_len == seq_cap) {            /* grow array if needed */
            seq_cap *= 2;
            seqarr = (seq_t *)realloc(seqarr, seq_cap * sizeof(seq_t));
        }
        seqarr[seq_len] = total;
        ++seq_len;

        /* prepare for next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }
}

/* ------------------------------------------------------------------ */
/* ----------- public function as required by the problem ----------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset starts at 1 */

    extend_sequence(n);
    return seqarr[n];
}
