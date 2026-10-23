/*
 * A125360  Number of base 23 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = trace(A^n)  (n ≥ 0)
 * where A is the 23 × 23 adjacency matrix of the graph
 * with vertices 0…22 and an edge between i and j iff |i−j| ≤ 4.
 * The function returns a(0)=1 and for n>0 the number of base‑23
 * circular n‑digit numbers with adjacent digits differing by at most 4.
 */

#include <stddef.h>   /* size_t for loops */

/* constant parameters */
#define MAXD   23          /* size of the adjacency matrix   */
#define MAXPOW 64          /* enough for 2^63 (more than any n we care about) */

/* the adjacency powers: power[k] = A^(2^k) */
static seq_t power[MAXPOW][MAXD][MAXD];
static int power_count = 0;   /* number of powers currently computed */

/* ------------------------------------------------------------ */
/* matrix multiplication: r = a × b   (all dimensions = MAXD)  */
/* ------------------------------------------------------------ */
static void
mat_mul(const seq_t a[MAXD][MAXD], const seq_t b[MAXD][MAXD], seq_t r[MAXD][MAXD])
{
    for (size_t i = 0; i < MAXD; ++i) {
        for (size_t j = 0; j < MAXD; ++j) {
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k < MAXD; ++k)
                sum += a[i][k] * b[k][j];
            r[i][j] = sum;
        }
    }
}

/* ------------------------------------------------------------ */
/* make sure powers[0..ceil_log2(n)] are ready                  */
/* ------------------------------------------------------------ */
static void
ensure_powers(idx_t n)
{
    /* initialise the base adjacency matrix when needed */
    if (!power_count) {
        for (size_t i = 0; i < MAXD; ++i) {
            for (size_t j = 0; j < MAXD; ++j) {
                size_t diff = (i > j) ? i - j : j - i;
                power[0][i][j] = (diff <= 4) ? (seq_t)1 : (seq_t)0;
            }
        }
        power_count = 1;
    }

    /* find number of powers needed: smallest p with 2^p > n */
    idx_t needed = 0;
    while (((idx_t)1 << needed) <= n)
        ++needed;

    /* compute remaining powers on demand */
    for (int k = power_count; k < needed; ++k)
        mat_mul(power[k - 1], power[k - 1], power[k]);

    power_count = needed;
}

/* ------------------------------------------------------------ */
/* sequence function                                          */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* convention: empty product = 1 */

    ensure_powers(n);

    /* R = identity matrix */
    seq_t R[MAXD][MAXD];
    for (size_t i = 0; i < MAXD; ++i)
        for (size_t j = 0; j < MAXD; ++j)
            R[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    seq_t temp[MAXD][MAXD];

    /* multiply R by the powers that correspond to bits of n */
    for (int k = 0; k < power_count; ++k) {
        if (n & ((idx_t)1 << k)) {
            mat_mul(R, power[k], temp);
            /* copy temp -> R */
            for (size_t i = 0; i < MAXD; ++i)
                for (size_t j = 0; j < MAXD; ++j)
                    R[i][j] = temp[i][j];
        }
    }

    /* trace of R = trace(A^n) */
    seq_t ans = (seq_t)0;
    for (size_t i = 0; i < MAXD; ++i)
        ans += R[i][i];

    return ans;
}
