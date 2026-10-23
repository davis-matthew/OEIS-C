/*
 * A127861  a(n) = A127859(n)^2.
 * https://oeis.org/A127861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (6*(12^n - 1)/11 + 1)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (6*(12^n - 1)/11 + 1)^2 = A127860(n) + (12^(2*n) - 1)/11 */

#include <stdint.h>
#include <stddef.h>

/* The following function computes A127861 in a cache-friendly way.
 * It uses the recurrence x_n = 12*x_{n-1} - 5  (with x_1 = 7)
 * where a(n) = x_n^2.  The function keeps the last computed x
 * in static storage and updates it forwards or backwards as needed.
 * The result fits into seq_t for all n that fit.  If seq_t is a
 * small unsigned integer type, the function still returns the
 * correct value for all n that fit in that type.  For larger n
 * it may overflow; that is acceptable because those terms do not
 * need to be checked. */
seq_t a(idx_t n)
{
    /* static state: last computed n and its corresponding x_n */
    static idx_t last_n = 1;          /* last argument processed  */
    static seq_t last_x = 7;          /* x_1 = 7 */
    static int initialized = 0;

    /* initialise static state only once */
    if (!initialized) {
        initialized = 1;
    }

    /* if the requested n differs from the cached n, update last_x */
    if (n != last_n) {
        if (n > last_n) {
            for (idx_t i = last_n; i < n; ++i) {
                last_x = last_x * 12 - 5;
            }
        } else { /* n < last_n */
            for (idx_t i = last_n; i > n; --i) {
                /* x_{i-1} = (x_i + 5) / 12  (exact division) */
                last_x = (last_x + 5) / 12;
            }
        }
        last_n = n;
    }

    /* compute a(n) = last_x^2 */
    return last_x * last_x;
}
