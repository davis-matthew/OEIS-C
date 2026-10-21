/*
 * A097867  The n-th group (n>=0) of 9 consecutive terms are the entries, read
 *          by rows, of the 3 X 3 matrix A[n]=M*A[n-1], where M is the 3 X 3
 *          matrix [[0, 1, 0], [0, 0, 1], [1, 1, 0]] and A[0] is the 3 X 3
 *          matrix [[0, 1, 1], [1, 1, 2], [1, 2, 2]].
 * https://oeis.org/A097867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(9k + i) = element (i/3 , i%3) of the matrix A[k] where A[0] =
 *            [[0,1,1],[1,1,2],[1,2,2]] and A[n] = M * A[n-1], M =
 *            [[0,1,0],[0,0,1],[1,1,0]]. The terms are produced row‑wise
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for the sequence and matrix state */
static seq_t *seq      = NULL; /* dynamic array of terms          */
static size_t seq_cap  = 0;    /* capacity of seq                 */
static size_t seq_len  = 0;    /* number of terms stored          */
static size_t cur_grp  = 0;    /* last fully computed group index */
static seq_t cur_mat[3][3];    /* current A[cur_grp]               */

/* initialise the sequence and first matrix A[0] */
static void init_sequence(void)
{
    /* allocate initial buffer for first 9 terms */
    seq_cap = 64;
    seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    if (!seq) exit(1);

    /* set A[0] */
    cur_mat[0][0] = (seq_t)0;  cur_mat[0][1] = (seq_t)1;  cur_mat[0][2] = (seq_t)1;
    cur_mat[1][0] = (seq_t)1;  cur_mat[1][1] = (seq_t)1;  cur_mat[1][2] = (seq_t)2;
    cur_mat[2][0] = (seq_t)1;  cur_mat[2][1] = (seq_t)2;  cur_mat[2][2] = (seq_t)2;

    /* store its nine entries */
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            seq[seq_len++] = cur_mat[r][c];

    cur_grp = 0;
}

/* compute the next matrix A[cur_grp+1] and append its 9 terms */
static void compute_next(void)
{
    seq_t next[3][3];

    /* next = M * cur_mat */
    for (int j = 0; j < 3; ++j) {
        next[0][j] = cur_mat[1][j];
        next[1][j] = cur_mat[2][j];
        next[2][j] = cur_mat[0][j] + cur_mat[1][j];
    }

    /* copy back to cur_mat for subsequent steps */
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            cur_mat[r][c] = next[r][c];

    /* ensure capacity for 9 more terms */
    if (seq_len + 9 > seq_cap) {
        seq_cap = seq_cap * 2 + 9;
        seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
        if (!seq) exit(1);
    }

    /* append the nine entries */
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            seq[seq_len++] = cur_mat[r][c];

    ++cur_grp;
}

/* external function required by the build system */
seq_t a(idx_t n)
{
    /* initialise lazily */
    if (!seq) init_sequence();

    /* compute any missing groups up to the requested one */
    size_t target_grp = (size_t)(n / 9);
    while (cur_grp < target_grp)
        compute_next();

    return seq[n];
}
