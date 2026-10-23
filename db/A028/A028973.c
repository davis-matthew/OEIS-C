/*
 * A028973  Theta series of quadratic form with Gram matrix [ 7, 1, -3; 1, 3,
 *          1; -3, 1, 7 ].
 * https://oeis.org/A028973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Theta series of the quadratic form with Gram matrix
   [ 7,  1, -3;
     1,  3,  1;
    -3,  1,  7 ].
   a(n) = #{ (x,y,z) ∈ ℤ³ : 7x² + 2xy – 6xz + 3y² + 2yz + 7z² = n }.
   The implementation counts all representations by exhaustive
   enumeration inside a sphere bound given by the smallest
   eigenvalue of the Gram matrix (λ_min = 2). */

#include <stdlib.h>
#include <math.h>

static seq_t *count = NULL;   /* table of a(0)…a(max_n-1)          */
static idx_t   max_n  = 0;    /* size of table                       */

seq_t a(idx_t n)
{
    /* enlarge table if necessary */
    if (n >= max_n) {
        idx_t new_cap = n + 1;
        seq_t *new_count = (seq_t*)calloc((size_t)new_cap, sizeof(seq_t));
        if (!new_count) return 0;   /* out of memory */

        /* λ_min = 2 → each |xi| ≤ sqrt(n/2) */
        long double limit = (long double)n / 2.0L;
        long long  B = (long long)floor(sqrtl(limit)) + 1;  /* safety */

        for (long long x = -B; x <= B; ++x) {
            long long x2 = x * x;
            for (long long y = -B; y <= B; ++y) {
                long long y2 = y * y;
                for (long long z = -B; z <= B; ++z) {
                    long long z2 = z * z;
                    /* compute Q(x,y,z)  */
                    long long val =
                        7LL * x2
                      + 2LL * x * y
                      - 6LL * x * z
                      + 3LL * y2
                      + 2LL * y * z
                      + 7LL * z2;
                    if (val >= 0 && (idx_t)val < new_cap)
                        ++new_count[(idx_t)val];
                }
            }
        }

        free(count);
        count = new_count;
        max_n = new_cap;
    }
    return count[n];
}
