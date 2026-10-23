/*
 * A106542  a(n) = a(n-1) - 2*a(n-2) - 3*a(n-3) - ... - (n-1)*a(1), a(1) =
 *          a(2) = 3, a(3) = -3.
 * https://oeis.org/A106542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-2) + 2*a(n-3), a(1)=3, a(2)=3, a(3)=-3
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 32, i64 n <= 67, i128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static array to memoize computed values */
    static seq_t *data = NULL;
    static idx_t stored_max = 0;
    static size_t capacity = 0;

    /* Ensure array can hold index n (0 is unused) */
    if ((size_t)(n + 1) > capacity) {
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(data, newcap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort (problem context will handle it) */
            return 0;
        }
        data = tmp;
        capacity = newcap;
    }

    /* If the requested value is already computed, return it */
    if (n <= stored_max) {
        return data[n];
    }

    /* Compute values from stored_max+1 up to n */
    for (idx_t i = stored_max + 1; i <= n; ++i) {
        switch (i) {
            case 1:
                data[1] = (seq_t)3;
                break;
            case 2:
                data[2] = (seq_t)3;
                break;
            case 3:
                data[3] = (seq_t)(-3);
                break;
            default:
                data[i] = (seq_t)3 * data[i - 1]
                        - (seq_t)5 * data[i - 2]
                        + (seq_t)2 * data[i - 3];
                break;
        }
    }
    stored_max = n;
    return data[n];
}
