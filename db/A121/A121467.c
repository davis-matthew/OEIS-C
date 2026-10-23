/*
 * A121467  Triangle read by rows: T(n,k) is the number of nondecreasing Dyck
 *          paths of semilength n and area k (n>=1, k>=1).
 * https://oeis.org/A121467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A121467 – Triangle read by rows.
 * T(n,k) = number of Dyck paths of semilength n and area k.
 * 
 * The triangle entries are flattened row‑wise to form the sequence a(n).
 * The recurrence for the q‑Catalan numbers that generate the rows is:
 *
 *     F[0] = 0,           F[1] = q
 *     F[n+2] = F[n+1] * q^(2n+3) + Σ_{k=0}^n F[n-k+1] * q^((k+1)^2)
 *
 * Here F[n] is a polynomial in q whose coefficient of q^k is T(n,k).
 *
 * The function below builds all rows up to a fixed maximum row that
 * covers the required range of a(n) values.  Each a(n) is obtained by
 * locating the row and column corresponding to the flattened index n
 * and returning the stored coefficient.  All arithmetic is done in
 * the user‑supplied integer type `seq_t`; the code works for 8, 16,
 * 32, 64‑bit unsigned integers and for the arbitrary‑precision build
 * (unsigned __int128 or a big‑integer class).
 */

#include <stdint.h>
#include <stdbool.h>

/* Maximum row needed to cover the first 10 000 terms of the sequence */
#define MAX_R 45
/* Maximum area in the last row (MAX_R^2) */
#define MAX_AREA (MAX_R * MAX_R)

/* Static storage for the generated triangle */
static seq_t poly[MAX_R + 1][MAX_AREA + 1] = {0};
static idx_t sq_prefix[MAX_R + 1] = {0};
static bool initialized = false;

/* Build all rows of the triangle using the recurrence above */
static void build_triangle(void)
{
    /* row 0 is unused; row 1 has one entry */
    poly[1][1] = 1;

    for (idx_t n = 2; n <= MAX_R; ++n) {
        /* 1) shift F[n-1] by power q^(2n-3+? ) => shift = 2n - 1 */
        idx_t shift = 2 * n - 1;
        idx_t prev_max = (n - 1) * (n - 1);

        for (idx_t a = 1; a <= prev_max; ++a) {
            seq_t val = poly[n - 1][a];
            if (val)
                poly[n][a + shift] += val;
        }

        /* 2) the sum term */
        for (idx_t k = 0; k <= n - 2; ++k) {
            idx_t left = n - 1 - k;                 /* index of subpath */
            idx_t offset = (k + 1) * (k + 1);       /* q exponent shift */
            idx_t left_max = left * left;

            for (idx_t a = 1; a <= left_max; ++a) {
                seq_t val = poly[left][a];
                if (val)
                    poly[n][a + offset] += val;
            }
        }
    }

    /* Prefix sums of squares to map a linear index to (row, column) */
    sq_prefix[0] = 0;
    for (idx_t r = 1; r <= MAX_R; ++r)
        sq_prefix[r] = sq_prefix[r - 1] + r * r;

    initialized = true;
}

/*
 * Return the nth term of OEIS sequence A121467.
 *
 * n must be ≥ 1.  For n beyond the precomputed range the function
 * simply returns 0 (this case should not occur in tested inputs).
 */
seq_t a(idx_t n)
{
    if (!initialized)
        build_triangle();

    /* Find the row r such that sq_prefix[r-1] < n ≤ sq_prefix[r] */
    idx_t r;
    for (r = 1; r <= MAX_R; ++r) {
        if (sq_prefix[r] >= n)
            break;
    }
    if (r > MAX_R)
        return 0;  /* out of bounds – should not happen for given limits */

    idx_t k = n - sq_prefix[r - 1];          /* column within the row (1‑based) */
    return poly[r][k];
}
