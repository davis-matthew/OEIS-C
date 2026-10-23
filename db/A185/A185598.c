/*
 * A185598  Numbers not of the form floor(n^(3/2)-n^(1/2)); complement of
 *          A185597.
 * https://oeis.org/A185598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 66, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A185598 – numbers not of the form floor(n^(3/2)-n^(1/2)). */
/* Computation by direct search, using the identity
   floor(n^(3/2)-n^(1/2)) = floor( (n-1)*sqrt(n) )
   which equals floor(sqrt((n-1)^2 * n)).                                   */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t compute_f(idx_t m)
{
    if (m == 0) return 0;                 /* not used, but safe */
    seq_t a = (seq_t)(m - 1);              /* a = m-1 */
    return floor_sqrt(a * a * (seq_t)m);   /* floor(sqrt((m-1)^2 * m)) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0 */

    idx_t x = 1;           /* current integer being tested */
    idx_t count = 0;       /* how many missing numbers found so far */
    idx_t k = 1;           /* current index for f(k) */
    seq_t f_curr = compute_f(k);

    for (;;) {
        bool is_f = false;

        /* advance k while f(k) <= x, checking for equality */
        while (f_curr <= (seq_t)x) {
            if (f_curr == (seq_t)x) {
                is_f = true;
                break;                 /* x is in the image of f, skip */
            }
            ++k;
            f_curr = compute_f(k);
        }

        if (!is_f) {                       /* x is missing */
            ++count;
            if (count == n) return (seq_t)x;
        }

        ++x;                               /* next integer to test */
    }
}
