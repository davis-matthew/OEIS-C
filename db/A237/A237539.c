/*
 * A237539  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the lower
 *          median minus the upper median minus the minimum of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of entries of v0 * M^(n-1), where M is the 64x64
 *            transition matrix on states (row, last-label), v0 is the vector
 *            counting all first edges
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define STATES 64

static bool matrix_initialized = false;
static seq_t M[STATES][STATES];
static seq_t v0[STATES];

/* Pre‑compute the transition matrix M and initial vector v0.  */
static void initialize(void)
{
    if (matrix_initialized) return;
    /* label[row_a][row_b] = value of 2×2 block with top row=row_a, bottom row=row_b   */
    int label[16][16];
    for (int a = 0; a < 16; ++a) {
        int a0 = a / 4;
        int a1 = a % 4;
        for (int b = 0; b < 16; ++b) {
            int b0 = b / 4;
            int b1 = b % 4;
            int arr[4] = {a0, a1, b0, b1};
            /* simple 4‑element bubble sort */
            for (int i = 0; i < 4; ++i)
                for (int j = i + 1; j < 4; ++j)
                    if (arr[i] > arr[j]) {
                        int t = arr[i]; arr[i] = arr[j]; arr[j] = t;
                    }
            /* compute the expression max + lower median – upper median – min   */
            label[a][b] = arr[3] + arr[1] - arr[2] - arr[0];
        }
    }

    /* initialize M to 0 */
    for (int i = 0; i < STATES; ++i)
        for (int j = 0; j < STATES; ++j)
            M[i][j] = 0;

    /* build transitions */
    for (int src = 0; src < 16; ++src) {
        for (int dst = 0; dst < 16; ++dst) {
            int l = label[src][dst];
            for (int p = 0; p < 4; ++p) {
                if (p == 0 && l == 0) { /* will be handled below */
                }
                if ( (p == 0 && l == 0) || (p > 0 && l > 0) ) { /* placeholder */
                }
                /* check adjacency */
                if ( (p == 0 && l == 0) || (p == 0 && l == 1) ||
                     (p == 1 && l == 0) || (p == 1 && l == 2) ||
                     (p == 2 && l == 1) || (p == 2 && l == 3) ||
                     (p == 3 && l == 2) ) {
                    /* this check is equivalent to |p-l|==1 */
                }
                /* actually compute |p-l| */
                if (((p < l) ? (l - p) : (p - l)) == 1) {
                    int srcState = src * 4 + p;
                    int dstState = dst * 4 + l;
                    M[srcState][dstState] = M[srcState][dstState] + 1;
                }
            }
        }
    }

    /* build initial vector v0: count of first edges */
    for (int i = 0; i < STATES; ++i) v0[i] = 0;
    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            int l = label[a][b];
            int dstState = b * 4 + l;
            v0[dstState] = v0[dstState] + 1;
        }
    }

    matrix_initialized = true;
}

/* multiply two matrices A and B, store in C; all 64×64  */
static void mat_mul(seq_t C[STATES][STATES], const seq_t A[STATES][STATES],
                    const seq_t B[STATES][STATES])
{
    seq_t tmp[STATES][STATES];
    for (int i = 0; i < STATES; ++i) {
        for (int j = 0; j < STATES; ++j) {
            tmp[i][j] = 0;
            for (int k = 0; k < STATES; ++k)
                tmp[i][j] = tmp[i][j] + A[i][k] * B[k][j];
        }
    }
    for (int i = 0; i < STATES; ++i)
        for (int j = 0; j < STATES; ++j)
            C[i][j] = tmp[i][j];
}

/* multiply vector v (size STATES) by matrix A, store in res */
static void vec_mat_mul(seq_t res[STATES], const seq_t v[STATES],
                        const seq_t A[STATES][STATES])
{
    for (int j = 0; j < STATES; ++j) {
        res[j] = 0;
        for (int i = 0; i < STATES; ++i)
            res[j] = res[j] + v[i] * A[i][j];
    }
}

/* main function required by the exerciser */
seq_t a(idx_t n)
{
    initialize();

    if (n == 1) {
        seq_t sum = 0;
        for (int i = 0; i < STATES; ++i) sum = sum + v0[i];
        return sum;               /* 256 */
    }

    /* exponent is n-1 */
    idx_t e = n - 1;
    seq_t power[STATES][STATES];
    seq_t result_vec[STATES];
    seq_t temp_vec[STATES];

    /* copy M into power */
    for (int i = 0; i < STATES; ++i)
        for (int j = 0; j < STATES; ++j)
            power[i][j] = M[i][j];

    /* start with vector v0 */
    for (int i = 0; i < STATES; ++i) result_vec[i] = v0[i];

    while (e > 0) {
        if (e & 1) {
            vec_mat_mul(temp_vec, result_vec, power);
            for (int i = 0; i < STATES; ++i) result_vec[i] = temp_vec[i];
        }
        e >>= 1;
        if (e) mat_mul(power, power, power);   /* power = power * power */
    }

    /* sum entries of result_vec */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i) total = total + result_vec[i];
    return total;
}
