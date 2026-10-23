/*
 * A124804  Number of base 32 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A124804: Number of base‑32 circular n‑digit numbers with adjacent digits
    differing by at most 1.
    The sequence satisfies the linear recurrence of order 32

        a(n) = Σ_{i=1}^{32} r[i] · a(n-i)   for n ≥ 32

    where the coefficients r[1]…r[32] are derived from the eigenvalues of the
    32‑vertex path graph with self‑loops.  The first 32 values are pre‑computed
    by tracing powers of that adjacency matrix.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* number of digits in the alphabet */
#define K 32

static seq_t init_val[K];          /* a(0)…a(31) */
static int32_t rcoef[K];           /* r[0]…r[31] corresponding to a(n-i) */
static bool ready = false;

/* ------------------------------------------------------------------ */
/*  Build the recurrence coefficients and the initial values          */
/* ------------------------------------------------------------------ */
static void build_tables(void)
{
    /* --------------------------------------------------------------- */
    /*  Compute the coefficients r[i] from the characteristic
        polynomial Q_K(λ) of the path graph.                                */
    /* --------------------------------------------------------------- */
    const int64_t max_deg = K;
    int64_t prevprev[33] = {0};
    int64_t prev[33]     = {0};
    int64_t cur[33]      = {0};

    /* Q₀(λ) = 1 */
    prevprev[0] = 1;

    /* Q₁(λ) = λ - 1 */
    prev[0] = -1;
    prev[1] =  1;

    for (int k = 2; k <= max_deg; ++k) {
        /* Q_k(λ) = (λ-1)·Q_{k-1}(λ) - Q_{k-2}(λ) */
        for (int j = 0; j <= k; ++j) {
            int64_t a = (j>0)       ? prev[j-1] : 0;
            int64_t b = (j<k)       ? prev[j]   : 0;
            int64_t c = (j<k-1)     ? prevprev[j] : 0;
            cur[j] = a - b - c;
        }
        /* shift */
        for (int j = 0; j <= k; ++j) {
            prevprev[j] = prev[j];
            prev[j]     = cur[j];
        }
    }

    /* r[i] = -c_{K-i} where c_j are the coefficients of Q_K(λ)
       (i = 1 .. K).  prev[0]..prev[K] hold them. */
    for (int i = 1; i <= K; ++i)
        rcoef[i-1] = -(int32_t)prev[i-1];

    /* --------------------------------------------------------------- */
    /*  Pre‑compute a(0)…a(31) as trace(M^n) where M is the
        adjacency matrix of the 32‑vertex path graph with
        self‑loops.                                                     */
    /* --------------------------------------------------------------- */
    seq_t M[K][K] = {0};
    seq_t P[K][K] = {0};
    seq_t T[K][K];

    /* Build adjacency matrix M and identity matrix P */
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            if (i == j)          M[i][j] = 1;
            else if (j==i-1||j==i+1) M[i][j] = 1;
            else                  M[i][j] = 0;

            P[i][j] = (i==j)?1:0;
        }
    }

    init_val[0] = 1;                    /* definition of a(0) */
    for (int step = 1; step < K; ++step) {
        /* T = P * M */
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j) {
                seq_t sum = 0;
                for (int t = 0; t < K; ++t)
                    sum += P[i][t] * M[t][j];
                T[i][j] = sum;
            }
        /* copy T back to P */
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j)
                P[i][j] = T[i][j];

        /* trace(P) = a(step) */
        seq_t trace = 0;
        for (int i = 0; i < K; ++i)
            trace += P[i][i];
        init_val[step] = trace;
    }

    ready = true;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!ready)
        build_tables();

    if (n == 0)
        return init_val[0];
    if (n < K)
        return init_val[(size_t)n];

    seq_t last[K];
    for (int i = 0; i < K; ++i)
        last[i] = init_val[i];

    for (idx_t k = K; k <= n; ++k) {
        __int128 sum = 0;
        for (int i = 1; i <= K; ++i) {
            sum += (__int128)rcoef[i-1] * (__int128)last[(k - i) & (K-1)];
        }
        seq_t cur = (seq_t)sum;
        last[k & (K-1)] = cur;
    }

    return last[n & (K-1)];
}
