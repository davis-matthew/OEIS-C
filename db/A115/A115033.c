/*
 * A115033  Alternately multiply and divide, with a(1)=2 and a(2)=3.
 * https://oeis.org/A115033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 2, a(2) = 3. Recurrence: a(2k) = a(2k-3); a(2k+1) =
 *            a(2k)*a(2k-1). (k ≥ 1)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 14, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A115033.  */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    /* Allocate transient array of size n+1; index 0 unused. */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr) return (seq_t)0;           /* allocation failed */

    arr[1] = (seq_t)2;
    if (n == 1) {
        seq_t res = arr[1];
        free(arr);
        return res;
    }
    arr[2] = (seq_t)3;

    for (idx_t i = 3; i <= n; ++i) {
        if (i & 1) {                    /* odd index: a(2k+1) = a(2k)*a(2k-1) */
            arr[i] = arr[i - 1] * arr[i - 2];
        } else {                        /* even index: a(2k) = a(2k-3) */
            arr[i] = arr[i - 3];
        }
    }

    seq_t res = arr[n];
    free(arr);
    return res;
}
