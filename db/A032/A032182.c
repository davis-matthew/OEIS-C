/*
 * A032182  "CIJ" (necklace, indistinct, labeled) transform of 2,1,1,1...
 * https://oeis.org/A032182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n! * [x^n] log(1/(2 - x - exp(x)))
 * The sequence is computed by a recurrence on two auxiliary
 * integer sequences u and a (a(1)=2, u(0)=1):
 *
 *   u_m = 2*m*u_{m-1} + sum_{j=0}^{m-2} binom(m,j)*u_j          (m≥1)
 *   a_{m+1} = 2*u_m + sum_{j=0}^{m-1} binom(m,j)*u_j
 *
 * The binomial coefficients are formed iteratively to avoid
 * oversized intermediate values.  The implementation stores the
 * values in static arrays with a pre‑allocated maximum size
 * sufficient for the required tests (≈10000).
 *
 * The function accepts arbitrary‑precision or fixed‑size integer
 * types for seq_t and idx_t; all operations are performed on
 * seq_t values only.
 */
#include <stddef.h>
#include <stdbool.h>

/* Pre‑allocation: enough for n up to ~10000. */
#define MAX_N 11000   /* safety margin */

/* Global static tables for memoisation */
static seq_t a_tbl[MAX_N + 1];
static seq_t u_tbl[MAX_N];
static idx_t  max_u = 0;   /* highest computed index in u_tbl  (initially 0) */
static idx_t  max_a = 1;   /* highest computed index in a_tbl  (a[1] known) */
static bool   init_done = false;

/* Initialise base values, run once. */
static void init_tables(void)
{
    if (init_done) return;
    a_tbl[1] = 2;          /* a(1) = 2 */
    u_tbl[0] = 1;          /* u_0 = 1   */
    init_done = true;
}

/* Main function requested by the problem. */
seq_t a(idx_t n)
{
    /* Guard for n == 0 (undefined offset) */
    if (n == 0) return 0;

    /* Initialise tables on first call. */
    init_tables();

    if (n <= max_a)            /* already computed */
        return a_tbl[n];
    if (n >= (idx_t)MAX_N)     /* out of bounds for static array */
        return 0;              /* fallback – no overflow needed here */

    /* Compute new terms up to required index n. */
    for (idx_t m = max_u + 1; m <= n - 1; ++m) {
        /* Compute sum1 = Σ_{j=0}^{m-2} C(m,j) * u[j]  (only if m≥2) */
        seq_t sum1 = 0;
        seq_t binom = 1;           /* C(m,0) */
        for (idx_t j = 0; j < m - 1; ++j) {
            seq_t prod = u_tbl[j];
            prod *= binom;
            sum1 += prod;

            /* Update binom to C(m, j+1) */
            seq_t tmp = binom;
            tmp *= (m - j);          /* multiply by (m-j) */
            binom = tmp / (j + 1);   /* divide by (j+1) – divides exactly */
        }
        /* Now binom holds C(m, m-1) */
        seq_t sum2 = sum1;
        {
            seq_t prod = u_tbl[m - 1];
            prod *= binom;
            sum2 += prod;
        }

        /* Compute u[m] = 2*m*u[m-1] + sum1 */
        seq_t u_prev = u_tbl[m - 1];
        seq_t temp_u = u_prev;
        temp_u *= (seq_t)(2 * m);  /* multiply by small integer */
        temp_u += sum1;
        u_tbl[m] = temp_u;

        /* Compute a[m+1] = 2*u[m] + sum2 */
        seq_t temp_a = u_tbl[m];
        temp_a *= 2;               /* 2*u[m] */
        temp_a += sum2;
        a_tbl[m + 1] = temp_a;

        /* Update memoisation indices */
        max_u = m;
        max_a = m + 1;
    }

    return a_tbl[n];
}
