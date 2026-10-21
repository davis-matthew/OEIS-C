/*
 * A205982  Number of (n+1)X5 0..2 arrays with every 2X3 or 3X2 subblock
 *          having exactly two clockwise edge increases.
 * https://oeis.org/A205982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 84*a(n-3) for n > 9, a(1) … a(9) are computed by dynamic
 *            programming
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 25, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* non‑n incidence matrix for rows of length 5, values 0–2 */
#define ROWS 243                  /* 3^5 */
#define STATES (ROWS * ROWS)      /* (prevprev,prev) */

/* precomputed digits of every row (little endian) */
static unsigned char row_digit[ROWS][5];

/* ------------------------------------------------------------------ */
/*   Helpers that test the 2×3 and 3×2 clockwise‑edge‑increase rule   */
/* ------------------------------------------------------------------ */
static inline bool ok_2x3_sub(int top, int bottom, int col)
{
    int a = row_digit[top][col];
    int b = row_digit[top][col+1];
    int c = row_digit[top][col+2];
    int d = row_digit[bottom][col+2];
    int e = row_digit[bottom][col+1];
    int f = row_digit[bottom][col];
    int inc = 0;
    if (b>a) inc++;
    if (c>b) inc++;
    if (d>c) inc++;
    if (e>d) inc++;
    if (f>e) inc++;
    if (a>f) inc++;
    return inc == 2;
}

static inline bool ok_2x3_all(int top, int bottom)
{
    for (int col = 0; col <= 2; ++col)
        if (!ok_2x3_sub(top, bottom, col))
            return false;
    return true;
}

static inline bool ok_3x2_sub(int top, int mid, int bottom, int col)
{
    int t0 = row_digit[top][col];
    int t1 = row_digit[top][col+1];
    int m0 = row_digit[mid][col];
    int m1 = row_digit[mid][col+1];
    int b0 = row_digit[bottom][col];
    int b1 = row_digit[bottom][col+1];
    int inc = 0;
    if (t1>t0) inc++;
    if (m1>t1) inc++;
    if (b1>m1) inc++;
    if (b0>b1) inc++;
    if (m0>b0) inc++;
    if (t0>m0) inc++;
    return inc == 2;
}

static inline bool ok_3x2_all(int top, int mid, int bottom)
{
    for (int col = 0; col <= 3; ++col)
        if (!ok_3x2_sub(top, mid, bottom, col))
            return false;
    return true;
}

/* ------------------------------------------------------------------ */
/*   Transition tables:  for every (prevprev,prev) we store a list of */
/*   possible next rows that keep all constraints satisfied.         */
/* ------------------------------------------------------------------ */
static unsigned char *adjacency_data = NULL;   /* concatenated lists */
static int            *adjacency_start = NULL; /* start indices */
static unsigned char *adjacency_count = NULL;   /* number of entries */
static bool            adjacency_built = false;

static void build_adjacency(void)
{
    if (adjacency_built)
        return;

    adjacency_start = (int*)malloc(STATES * sizeof(int));
    adjacency_count = (unsigned char*)malloc(STATES * sizeof(unsigned char));

    /* first pass – compute counts and total size */
    int total_entries = 0;
    for (int pp = 0; pp < ROWS; ++pp)
        for (int p = 0; p < ROWS; ++p) {
            int id = pp * ROWS + p;
            int cnt = 0;
            for (int cur = 0; cur < ROWS; ++cur)
                if (ok_2x3_all(p, cur) && ok_3x2_all(pp, p, cur))
                    ++cnt;
            adjacency_count[id] = (unsigned char)cnt;
            total_entries += cnt;
        }

    adjacency_data = (unsigned char*)malloc(total_entries * sizeof(unsigned char));

    /* second pass – fill the lists */
    int offset = 0;
    for (int pp = 0; pp < ROWS; ++pp)
        for (int p = 0; p < ROWS; ++p) {
            int id = pp * ROWS + p;
            adjacency_start[id] = offset;
            for (int cur = 0; cur < ROWS; ++cur)
                if (ok_2x3_all(p, cur) && ok_3x2_all(pp, p, cur))
                    adjacency_data[offset++] = (unsigned char)cur;
        }

    adjacency_built = true;
}

/* ------------------------------------------------------------------ */
/*   The first nine terms are pre‑computed by dynamic programming.    */
/* ------------------------------------------------------------------ */
static seq_t base_vals[10]; /* 1..9 used – 0 unused */
static bool   base_built = false;

static void compute_base_values(void)
{
    if (base_built)
        return;

    /* decode rows into base‑3 digits */
    for (int id = 0; id < ROWS; ++id) {
        int tmp = id;
        for (int j = 4; j >= 0; --j) {
            row_digit[id][j] = tmp % 3;
            tmp /= 3;
        }
    }

    build_adjacency();

    seq_t *dp_cur = (seq_t*)malloc(STATES * sizeof(seq_t));
    seq_t *dp_nxt = (seq_t*)malloc(STATES * sizeof(seq_t));

    /* initialise for 2 rows (a(1)) */
    for (int a = 0; a < STATES; ++a)
        dp_cur[a] = 0;
    for (int r1 = 0; r1 < ROWS; ++r1)
        for (int r2 = 0; r2 < ROWS; ++r2) {
            int id = r1 * ROWS + r2;
            dp_cur[id] = ok_2x3_all(r1, r2) ? 1 : 0;
        }

    /* a(1) */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i)
        total += dp_cur[i];
    base_vals[1] = total;

    /* compute a(2)…a(9) by extending up to 10 rows (n+1 = 10) */
    for (int rows = 3; rows <= 10; ++rows) {
        for (int i = 0; i < STATES; ++i)
            dp_nxt[i] = 0;

        for (int id = 0; id < STATES; ++id) {
            seq_t ways = dp_cur[id];
            if (!ways)
                continue;
            int pp = id / ROWS; /* row r-2 */
            int p  = id % ROWS; /* row r-1 */
            int start = adjacency_start[id];
            int cnt   = adjacency_count[id];
            for (int k = 0; k < cnt; ++k) {
                int cur = adjacency_data[start + k];
                int nid = p * ROWS + cur;  /* new state (prevprev=p,prev=cur) */
                dp_nxt[nid] += ways;
            }
        }

        total = 0;
        for (int i = 0; i < STATES; ++i)
            total += dp_nxt[i];
        base_vals[rows - 1] = total;      /* r-1 == n */

        /* swap */
        seq_t *tmp = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;
    }

    free(dp_cur);
    free(dp_nxt);
    base_built = true;
}

/* ------------------------------------------------------------------ */
/*   Public function                                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    compute_base_values();          /* guarantees base_vals[1..9] are ready */

    if (n <= 9)
        return base_vals[(int)n];

    /* reduce n until it is ≤ 9 using the recurrence a(n)=84*a(n−3) */
    idx_t rem = n;
    unsigned long k = 0;
    while (rem > 9) {          /* safe: k fits in unsigned long */
        rem -= 3;
        ++k;
    }

    seq_t result = base_vals[(int)rem];
    for (unsigned long i = 0; i < k; ++i)
        result *= (seq_t)84;

    return result;
}
