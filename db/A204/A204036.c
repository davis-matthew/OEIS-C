/*
 * A204036  Number of (n+1)X6 0..1 arrays with the sums of 2X2 subblocks
 *          nondecreasing rightwards and downwards.
 * https://oeis.org/A204036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is counted by dynamic programming over pairs of 6‑bit rows.
 *            * Each state is an ordered pair (p,q) of consecutive rows with
 *            the * 5‑vector S of 2×2 sums nondecreasing left‑to‑right. *
 *            Transitions from (p,q) to (q,r) are allowed iff each component
 *            of S * is also nondecreasing top‑to‑bottom. *...
 * Range:     u8 none, u16 n <= 3, u32 n <= 12, u64 n <= 42, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------------ */
/* Types supplied by the build -------------------------------------- */
typedef idx_t idx_t;
typedef seq_t seq_t;

/* ------------------------------------------------------------------ */
/* Helper memory allocation routines -------------------------------- */
static inline seq_t *alloc_seq_array(size_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t*)malloc(sizeof(seq_t) * n);
#endif
}
static inline int *alloc_int_array(size_t n) {
#ifdef __cplusplus
    return new int[n];
#else
    return (int*)malloc(sizeof(int) * n);
#endif
}
static inline unsigned char *alloc_uchar_array(size_t n) {
#ifdef __cplusplus
    return new unsigned char[n];
#else
    return (unsigned char*)malloc(sizeof(unsigned char) * n);
#endif
}
static inline void free_seq_arr(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}
static inline void free_int_arr(int *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}
static inline void free_uchar_arr(unsigned char *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* ------------------------------------------------------------------ */
/* State data: all 6‑bit row combinations (0..63) ------------------- */
#define MAX_ROWS 64
#define MAX_STATES 4096   /* 64*64 */

/* static arrays for state enumeration */
static unsigned char state_p[MAX_STATES];
static unsigned char state_q[MAX_STATES];
static unsigned char state_s[MAX_STATES][5]; /* 2×2 sums for a pair */

/* number of valid states after filtering */
static int num_states = 0;

/* -------------------------- transitions ---------------------------- */
static int *adj_start;   /* start index in adj_data for each state */
static int *adj_len;     /* number of successors for each state */
static int *adj_data;    /* concatenated list of successors */

/* --------------------------- DP ------------------------------- */
static seq_t *dp_cur;
static seq_t *dp_next;

/* --------------------- memoisation ------------------------------ */
static seq_t *memo = NULL;
static size_t memo_cap = 0;   /* allocated capacity (number of seq_t slots) */
static idx_t last_n = 0;      /* largest n for which a(n) is stored (>=1) */

/* ------------------------------------------------------------------ */
/* Initialise all static data (once) -------------------------------- */
static void init(void)
{
    /* ---------------------------------------------------------------- */

    /* 1. generate all valid (p,q) pairs (states) */
    int state_map[MAX_ROWS][MAX_ROWS];
    for (int i = 0; i < MAX_ROWS; ++i)
        for (int j = 0; j < MAX_ROWS; ++j)
            state_map[i][j] = -1;

    for (int p = 0; p < MAX_ROWS; ++p) {
        for (int q = 0; q < MAX_ROWS; ++q) {
            unsigned char s[5];
            int ok = 1;
            for (int j = 0; j < 5; ++j) {
                s[j] = ((p >> j) & 1) + ((p >> (j + 1)) & 1) +
                       ((q >> j) & 1) + ((q >> (j + 1)) & 1);
                if (j > 0 && s[j - 1] > s[j]) {
                    ok = 0;
                    break;
                }
            }
            if (!ok)
                continue;
            state_map[p][q] = num_states;
            state_p[num_states] = (unsigned char)p;
            state_q[num_states] = (unsigned char)q;
            memcpy(state_s[num_states], s, 5);
            ++num_states;
        }
    }
    /* ---------------------------------------------------------------- */

    /* 2. build adjacency lists */
    adj_len = alloc_int_array(num_states);
    memset(adj_len, 0, sizeof(int) * num_states);

    int total_edges = 0;
    for (int src = 0; src < num_states; ++src) {
        unsigned char q = state_q[src];
        for (int dst = 0; dst < num_states; ++dst) {
            if (state_p[dst] != q)
                continue;
            int ok = 1;
            for (int j = 0; j < 5; ++j) {
                if (state_s[dst][j] < state_s[src][j]) {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                ++adj_len[src];
                ++total_edges;
            }
        }
    }

    adj_start = alloc_int_array(num_states);
    adj_start[0] = 0;
    for (int i = 1; i < num_states; ++i)
        adj_start[i] = adj_start[i - 1] + adj_len[i - 1];

    adj_data = alloc_int_array(total_edges);
    int *ptr = alloc_int_array(num_states);
    memcpy(ptr, adj_start, sizeof(int) * num_states);
    for (int src = 0; src < num_states; ++src) {
        unsigned char q = state_q[src];
        for (int dst = 0; dst < num_states; ++dst) {
            if (state_p[dst] != q)
                continue;
            int ok = 1;
            for (int j = 0; j < 5; ++j) {
                if (state_s[dst][j] < state_s[src][j]) {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                adj_data[ptr[src]++] = dst;
            }
        }
    }
    free_int_arr(ptr);

    /* ---------------------------------------------------------------- */

    /* 3. allocate DP arrays */
    dp_cur  = alloc_seq_array(num_states);
    dp_next = alloc_seq_array(num_states);
    for (int i = 0; i < num_states; ++i)
        dp_cur[i] = (seq_t)1;   /* a(1) counts each state once */
    memset(dp_next, 0, sizeof(seq_t) * num_states);

    /* compute a(1) */
    seq_t sum = (seq_t)0;
    for (int i = 0; i < num_states; ++i)
        sum += dp_cur[i];

    /* 4. initialise memoisation */
    memo_cap = 64;    /* initial capacity */
    memo = alloc_seq_array(memo_cap);
    memo[0] = (seq_t)0;       /* unused slot */
    memo[1] = sum;            /* a(1) */
    last_n = 1;
}

/* ------------------------------------------------------------------ */
/* expand memoisation table if needed -------------------------------- */
static void memo_grow(size_t new_cap)
{
    if (new_cap <= memo_cap)
        return;
    seq_t *new_mem = alloc_seq_array(new_cap);
    for (size_t i = 0; i < memo_cap; ++i)
        new_mem[i] = memo[i];
    free_seq_arr(memo);
    memo = new_mem;
    memo_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* perform one DP step: move from n to n+1 ---------------------------- */
static void dp_step(void)
{
    /* zero dp_next */
    memset(dp_next, 0, sizeof(seq_t) * num_states);

    /* propagate counts */
    for (int src = 0; src < num_states; ++src) {
        seq_t val = dp_cur[src];
        if (val == (seq_t)0)
            continue;
        int start = adj_start[src];
        int end   = start + adj_len[src];
        for (int k = start; k < end; ++k) {
            int dst = adj_data[k];
            dp_next[dst] += val;
        }
    }

    /* swap buffers */
    seq_t *tmp = dp_cur;
    dp_cur = dp_next;
    dp_next = tmp;

    /* compute total for current n */
    seq_t total = (seq_t)0;
    for (int i = 0; i < num_states; ++i)
        total += dp_cur[i];

    /* store in memo */
    idx_t next_n = last_n + 1;          /* n of the result just computed */
    memo_grow((size_t)next_n + 1);      /* ensure capacity for index next_n */
    memo[next_n] = total;
    last_n = next_n;
}

/* ------------------------------------------------------------------ */
/* ensure that a(n) has been computed -------------------------------- */
static void ensure_up_to(idx_t n)
{
    if (!last_n)            /* lazy initialisation */
        init();
    while (last_n < n)
        dp_step();
}

/* ------------------------------------------------------------------ */
/* required function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;    /* defined behaviour for n < 1 is not required */
    ensure_up_to(n);
    return memo[n];
}
