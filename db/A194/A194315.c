/*
 * A194315  Triangular array: g(n,k)=number of fractional parts (i*sqrt(6)) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n^2, 1<=k<=n.
 * https://oeis.org/A194315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) counts the number of i such that the fractional part of *
 *            i*sqrt(6) lies in the interval [(k-1)/r , k/r] where r and k are
 *            the * row and column of the triangular array extracted from the
 *            index n
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation evaluates the required bin counts by iterating
 *  over all i from 1 to r^2 for the requested row r and counting only
 *  the column of interest.  The product i*sqrt(6) is handled without
 *  floating‑point arithmetic by using a fixed‑point representation
 *  of sqrt(6) with a scaling factor 2^30.  The same technique is used
 *  for the product n*i*sqrt(6).  The algorithm costs O(r^2) operations,
 *  which is below the 2‑second limit for r ≤ 10 000. */

#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

/* Scale factor for the fixed‑point approximation of sqrt(6). */
static const unsigned long long SCALE = 1ULL << 30;   /* 2^30 */

/* Fallback: compute the scaled fractional part of sqrt(6)-2 exactly once. */
static unsigned long long f_scaled = 0;
static bool f_scaled_inited = false;

static void init_f_scaled(void)
{
    if (f_scaled_inited) return;
    long double sqrt6 = sqrtl(6.0L);
    f_scaled = (unsigned long long)((sqrt6 - 2.0L) * (long double)SCALE + 1e-12L);
    f_scaled_inited = true;
}

seq_t a(idx_t n_index)
{
    /* Find the row r such that the flattened index n falls in that row. */
    double t = sqrt(8.0 * (double)n_index + 1.0);
    idx_t r = (idx_t)((t - 1.0) / 2.0);
    if (r * (r + 1) / 2 < n_index) ++r;          /* ensure r*(r+1)/2 >= n_index */

    /* Column c within the row (1‑based). */
    idx_t c = n_index - r * (r - 1) / 2;

    /* Ensure f_scaled is ready. */
    init_f_scaled();

    /* Compute floor(n*sqrt(6)) and its fractional part for this row. */
    long double n_alpha = (long double)r * sqrtl(6.0L);
    unsigned long long floor_n_alpha = (unsigned long long)n_alpha;
    unsigned long long f_n_scaled =
        (unsigned long long)((n_alpha - (long double)floor_n_alpha) *
                             (long double)SCALE + 1e-12L);

    /* Variables for the recurrence. */
    unsigned long long a_val = 0;            /* floor(i*sqrt(6))    */
    unsigned long long p_scaled = 0;         /* scaled fractional part of i*sqrt(6) */
    unsigned long long b_val = 0;            /* floor(i*r*sqrt(6))  */
    unsigned long long p_bn_scaled = 0;      /* scaled fractional part of i*r*sqrt(6) */
    unsigned long long count = 0;

    idx_t limit = r * r;                     /* i goes up to r^2 */

    for (idx_t i = 1; i <= limit; ++i)
    {
        /* Update a_val and its fractional part. */
        unsigned long long delta_a = (p_scaled + f_scaled >= SCALE) ? 1ULL : 0ULL;
        a_val += 2ULL + delta_a;
        p_scaled = (p_scaled + f_scaled) & (SCALE - 1ULL);

        /* Update b_val and its fractional part. */
        unsigned long long delta_b = (p_bn_scaled + f_n_scaled >= SCALE) ? 1ULL : 0ULL;
        b_val += floor_n_alpha + delta_b;
        p_bn_scaled = (p_bn_scaled + f_n_scaled) & (SCALE - 1ULL);

        /* Difference determines the column index (0‑based). */
        unsigned long long diff = b_val - r * a_val;
        if (diff == (c - 1ULL))
            ++count;
    }

    return (seq_t)count;
}
