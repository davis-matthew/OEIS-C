/*
 * A065231  Fill a triangular array by rows by writing numbers 1 up to b(0), 1
 *          up to b(1), etc., where b(n) are the heptagonal numbers (A000566).
 *          The first elements of the rows form a(n).
 * https://oeis.org/A065231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + n minus the next heptagonal number * when that
 *            sum exceeds it
 * Range:     u8 n <= 41, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   heptagonal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/heptagonal.h"

static seq_t *arr = NULL;       /* cache of a(n) values */
static idx_t  max_n = (idx_t)-1; /* last computed index in arr */
static idx_t  k_next = 0;       /* index of next heptagonal number to subtract */

seq_t a(idx_t n)
{
    if (n == 0)              /* a(0) is defined as 1 */
        return (seq_t)1;

    /* allocate / grow cache if necessary */
    if (arr == NULL || n > max_n) {
        size_t new_size = (size_t)n + 1;          /* allocate up to n */
        if (arr == NULL) {
            arr = (seq_t *)malloc(new_size * sizeof(seq_t));
            max_n = 0;
            k_next = 0;
            arr[0] = (seq_t)1;
        } else {
            arr = (seq_t *)realloc(arr, new_size * sizeof(seq_t));
        }
        /* compute values from max_n+1 up to n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t val = arr[i - 1] + (seq_t)i;
            seq_t h = heptagonal(k_next);
            if (val > h) {
                val -= h;
                ++k_next;
            }
            arr[i] = val;
        }
        max_n = n;
    }
    return arr[(size_t)n];
}
