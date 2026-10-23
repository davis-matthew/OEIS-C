/*
 * A184557  Number of (n+1)X3 0..2 arrays with no 2X2 subblock summing to less
 *          than 4.
 * https://oeis.org/A184557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 tables over {0,1,2} with every * 2×2
 *            subblock sum ≥ 4. Computed as sum of all entries of Mⁿ, * where
 *            M is the 27×27 adjacency matrix of allowed row pairs
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define ROWS 27
#define COLUMNS 3
#define POW_BITS 15  /* 2^15 = 32768 > maximum n (≤ 10000) */

/* Each row is encoded in base 3: 0,1,2.  For index i (0‑26) we store
 * its three entries in rows[i][k]. */
static unsigned char rows[ROWS][COLUMNS];

/* Static matrices */
static seq_t powM[POW_BITS][ROWS][ROWS];
static bool initialized = false;

/* Helper to multiply two matrices: dest = A * B. */
static void mat_mul(const seq_t A[ROWS][ROWS],
                    const seq_t B[ROWS][ROWS],
                    seq_t dest[ROWS][ROWS])
{
    seq_t temp[ROWS][ROWS] = {{0}};
    for (idx_t i = 0; i < ROWS; ++i) {
        for (idx_t j = 0; j < ROWS; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < ROWS; ++k)
                sum += A[i][k] * B[k][j];
            temp[i][j] = sum;
        }
    }
    /* copy back to dest */
    for (idx_t i = 0; i < ROWS; ++i)
        for (idx_t j = 0; j < ROWS; ++j)
            dest[i][j] = temp[i][j];
}

/* Initialise rows and powM structures. */
static void init(void)
{
    if (initialized)
        return;

    /* Enumerate all 27 rows */
    for (idx_t i = 0; i < ROWS; ++i) {
        idx_t x = i;
        for (int k = COLUMNS - 1; k >= 0; --k) {
            rows[i][k] = (unsigned char)(x % 3);
            x /= 3;
        }
    }

    /* Base adjacency matrix M in powM[0] */
    for (idx_t i = 0; i < ROWS; ++i)
        for (idx_t j = 0; j < ROWS; ++j) {
            bool ok = true;
            for (int k = 0; k < 2 && ok; ++k) {
                unsigned int s =
                    rows[i][k] + rows[i][k + 1] +
                    rows[j][k] + rows[j][k + 1];
                if (s < 4)
                    ok = false;
            }
            powM[0][i][j] = ok ? 1 : 0;
        }

    /* Precompute M^(2^k) */
    for (int k = 1; k < POW_BITS; ++k)
        mat_mul(powM[k-1], powM[k-1], powM[k]);

    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init();                 /* ensure matrices ready */

    /* Identity matrix */
    seq_t res[ROWS][ROWS] = {{0}};
    for (idx_t i = 0; i < ROWS; ++i)
        res[i][i] = 1;

    /* Exponentiation by squaring using precomputed powers */
    idx_t exp = n;
    int bit = 0;
    while (exp) {
        if (exp & 1U)
            mat_mul(res, powM[bit], res);
        exp >>= 1;
        ++bit;
    }

    /* Sum all entries of res to get a(n) */
    seq_t total = 0;
    for (idx_t i = 0; i < ROWS; ++i)
        for (idx_t j = 0; j < ROWS; ++j)
            total += res[i][j];

    return total;
}
