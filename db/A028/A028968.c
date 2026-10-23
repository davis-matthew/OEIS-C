/*
 * A028968  Numbers represented by quadratic form with Gram matrix [ 2, 1, 1;
 *          1, 2, 1; 1, 1, 3 ].
 * https://oeis.org/A028968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   estimate_B
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028968:
   Numbers represented by the quadratic form  
   Q(x,y,z)=2x²+2y²+3z²+2xy+2xz+2yz.  The sequence lists all non‑negative
   integers occurring as Q(x,y,z) for some integers x,y,z, in increasing
   order.  The algorithm generates all representations up to a bound B
   that contains at least n distinct values, then returns the (n‑1)‑th
   element of the sorted list.  It uses a proven bound on |x|,|y| and
   for each (x,y) determines the exact z‑interval satisfying Q≤B. */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/estimate_B.h"

static seq_t *seq_vals   = NULL;   /* sorted array of representable numbers */
static size_t  seq_len   = 0;      /* number of elements stored */

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Generate all numbers Q(x,y,z) ≤ B and store the distinct values
   in a sorted array seq_vals.  This function discards any previous
   contents of seq_vals.  It uses bounds:
          |x|,|y| ≤ ⌊sqrt(B/2)⌋+1
   For each (x,y) it determines the exact integer interval of z
   satisfying Q≤B by solving the quadratic inequality. */
static void generate_up_to_B(size_t B)
{
    /* present[i]==1 iff some (x,y,z) gives Q=i */
    unsigned char *present = (unsigned char*)calloc(B + 1, 1);
    if (!present) exit(EXIT_FAILURE);

    int max_xy   = (int)(sqrt((double)B / 2.0)) + 1;   /* bound for |x|,|y| */

    for (int x = -max_xy; x <= max_xy; ++x) {
        long long x_ll  = (long long)x;
        long long x2    = x_ll * x_ll;
        long long ax2   = 2 * x2;
        for (int y = -max_xy; y <= max_xy; ++y) {
            long long y_ll  = (long long)y;
            long long y2    = y_ll * y_ll;
            long long ax2y2 = ax2 + 2 * y2;            /* 2x^2 + 2y^2 */

            long long s     = (long long)x + y;       /* x + y        */
            long long C     = 2 * (x2 + x_ll * y_ll + y2); /* 2x^2+2xy+2y^2 */

            /* Solve 3z² + 2s z + (C - B) ≤ 0 for integer z. */
            long double sp   = (long double)s;
            long double Cp   = (long double)C;
            long double Bp   = (long double)B;

            long double discr = 4.0L * sp * sp - 12.0L * (Cp - Bp);
            if (discr < 0) continue;

            long double sqrtD = sqrtl(discr);
            long double zlow  = (-2.0L * sp - sqrtD) / 6.0L;
            long double zhigh = (-2.0L * sp + sqrtD) / 6.0L;

            long long zmin = (long long)ceill(zlow - 1e-12L);
            long long zmax = (long long)floorl(zhigh + 1e-12L);
            if (zmin > zmax) continue;

            for (long long z = zmin; z <= zmax; ++z) {
                long long z_ll = z;
                long long z2  = z_ll * z_ll;
                seq_t Q = (seq_t)(ax2y2 + 2 * x_ll * y_ll + 3 * z2
                                 + 2 * x_ll * z_ll + 2 * y_ll * z_ll);
                if (Q <= B) present[(size_t)Q] = 1;
            }
        }
    }

    /* Count distinct values to allocate an array. */
    size_t new_len = 0;
    for (size_t i = 0; i <= B; ++i) if (present[i]) ++new_len;

    seq_t *new_arr = (seq_t*)malloc(new_len * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (size_t i = 0; i <= B; ++i) if (present[i]) new_arr[idx++] = (seq_t)i;
    free(present);

    free(seq_vals);          /* discard old data */
    seq_vals = new_arr;
    seq_len  = new_len;
}

/* --------------------------------------------------------------------- */
/* Ensure that seq_vals contains at least n elements.  If not, increase
   the bound B until it does.  The generated values are sorted in
   increasing order. */
static void ensure_filled(idx_t n)
{
    if (seq_len >= n) return;

    size_t B = estimate_B(n * 2);
    for (;;) {
        generate_up_to_B(B);
        if (seq_len >= n) break;
        B *= 2;             /* enlarge bound if not enough values */
    }
}

/* --------------------------------------------------------------------- */
/* Public sequence function.  a(1) = 0, a(2) = 2, a(3) = 3, ... */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour below offset is irrelevant */
    ensure_filled(n);
    return seq_vals[n - 1];
}
