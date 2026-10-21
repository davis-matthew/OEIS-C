/*
 * A209042  Number of n X 3 0..3 arrays with no element equal the average of
 *          immediate neighbors vertically above and horizontally left of it.
 * https://oeis.org/A209042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A209042: number of n × 3 arrays with entries 0..3 such that no
 * element equals the average of its immediate vertical upper neighbour
 * and horizontal left neighbour.
 *
 * The value is computed using a 64 × 64 transition matrix TRANS that
 * records admissible row transitions.  The 36 admissible first rows are
 * encoded in the vector A0.  For n ≥ 1
 *
 *     a(n) = 1ᵀ · A0 · (TRANS)^(n‑1)
 *
 * (for n = 1 we simply sum A0).  The code uses binary exponentiation of
 * the matrix and works for any integer type seq_t supplied by the
 * build environment.  It is valid C11 and C++17. */

#include <stdbool.h>

/* --- constants ---------------------------------------------------------- */
#define STATE_COUNT 64

/* --- static data -------------------------------------------------------- */
static seq_t A0[STATE_COUNT];
static seq_t TRANS[STATE_COUNT][STATE_COUNT];
static seq_t sumA0;          /* sum of all A0 entries (a(1));            */
static bool initialized = false;

/* --- build the first‑row vector and transition matrix ------------------ */
static void build_transition_table(void)
{
    seq_t zero = (seq_t)0;
    seq_t one  = (seq_t)1;

    /* initialise A0 and TRANS with zeros */
    for (int i = 0; i < STATE_COUNT; ++i) {
        A0[i] = zero;
        for (int j = 0; j < STATE_COUNT; ++j)
            TRANS[i][j] = zero;
    }

    /* admissible first rows: q1 != q0, q2 != q1 */
    for (int pat = 0; pat < STATE_COUNT; ++pat) {
        int q0 = pat & 3;           /* least significant 2 bits    */
        int q1 = (pat >> 2) & 3;    /* next 2 bits                */
        int q2 = (pat >> 4) & 3;    /* most significant 2 bits    */
        if (q1 != q0 && q2 != q1)
            A0[pat] = one;
    }

    /* compute sumA0 (used for a(1)) */
    sumA0 = zero;
    for (int i = 0; i < STATE_COUNT; ++i)
        sumA0 += A0[i];

    /* transition matrix for rows ≥ 2 */
    for (int prev = 0; prev < STATE_COUNT; ++prev) {
        int p0 = prev & 3;
        int p1 = (prev >> 2) & 3;
        int p2 = (prev >> 4) & 3;
        for (int cur = 0; cur < STATE_COUNT; ++cur) {
            int q0 = cur & 3;
            int q1 = (cur >> 2) & 3;
            int q2 = (cur >> 4) & 3;
            bool ok = true;

            /* column 0: cannot equal upper neighbour */
            if (q0 == p0) ok = false;

            /* column 1: cannot equal average of upper and left */
            if (ok) {
                int sum = p1 + q0;
                if ((sum & 1) == 0) {
                    int avg = sum >> 1;
                    if (q1 == avg) ok = false;
                }
            }

            /* column 2: similar to column 1 */
            if (ok) {
                int sum = p2 + q1;
                if ((sum & 1) == 0) {
                    int avg = sum >> 1;
                    if (q2 == avg) ok = false;
                }
            }

            TRANS[prev][cur] = ok ? one : zero;
        }
    }
}

/* --- vector × matrix multiplication ------------------------------------ */
static void vec_mul_mat(seq_t *res,
                        const seq_t *v,
                        const seq_t mat[STATE_COUNT][STATE_COUNT])
{
    seq_t zero = (seq_t)0;
    /* initialise result vector with zeros */
    for (int j = 0; j < STATE_COUNT; ++j)
        res[j] = zero;

    for (int k = 0; k < STATE_COUNT; ++k) {
        seq_t vk = v[k];
        if (vk == zero)
            continue;
        for (int j = 0; j < STATE_COUNT; ++j) {
            res[j] += vk * mat[k][j];
        }
    }
}

/* --- matrix × matrix multiplication ------------------------------------ */
static void mat_mul_mat(seq_t out[STATE_COUNT][STATE_COUNT],
                        const seq_t a[STATE_COUNT][STATE_COUNT],
                        const seq_t b[STATE_COUNT][STATE_COUNT])
{
    seq_t zero = (seq_t)0;
    seq_t tmp[STATE_COUNT][STATE_COUNT];

    /* initialise tmp with zeros */
    for (int i = 0; i < STATE_COUNT; ++i)
        for (int j = 0; j < STATE_COUNT; ++j)
            tmp[i][j] = zero;

    for (int i = 0; i < STATE_COUNT; ++i) {
        for (int k = 0; k < STATE_COUNT; ++k) {
            seq_t aik = a[i][k];
            if (aik == zero)
                continue;
            for (int j = 0; j < STATE_COUNT; ++j) {
                tmp[i][j] += aik * b[k][j];
            }
        }
    }

    /* copy tmp to out */
    for (int i = 0; i < STATE_COUNT; ++i)
        for (int j = 0; j < STATE_COUNT; ++j)
            out[i][j] = tmp[i][j];
}

/* --- sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise transition data on first use */
    if (!initialized) {
        build_transition_table();
        initialized = true;
    }

    if (n < 1)
        return (seq_t)0;

    if (n == 1)
        return sumA0;

    /* start with vector A0 */
    seq_t vec[STATE_COUNT];
    for (int i = 0; i < STATE_COUNT; ++i)
        vec[i] = A0[i];

    /* initial power matrix = TRANS */
    seq_t power[STATE_COUNT][STATE_COUNT];
    for (int i = 0; i < STATE_COUNT; ++i)
        for (int j = 0; j < STATE_COUNT; ++j)
            power[i][j] = TRANS[i][j];

    idx_t exp = n - 1;
    while (exp > 0) {
        if (exp & (idx_t)1) {
            seq_t tmp_vec[STATE_COUNT];
            vec_mul_mat(tmp_vec, vec, power);
            for (int i = 0; i < STATE_COUNT; ++i)
                vec[i] = tmp_vec[i];
        }

        seq_t tmp_mat[STATE_COUNT][STATE_COUNT];
        mat_mul_mat(tmp_mat, power, power);
        for (int i = 0; i < STATE_COUNT; ++i)
            for (int j = 0; j < STATE_COUNT; ++j)
                power[i][j] = tmp_mat[i][j];

        exp >>= 1;
    }

    /* sum the resulting vector */
    seq_t result = (seq_t)0;
    for (int i = 0; i < STATE_COUNT; ++i)
        result += vec[i];

    return result;
}
