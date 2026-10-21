/*
 * A224251  Numbers, a(n) where binomial(a(n), 5n-1) == 0 (mod 5) and
 *          binomial(a(n), k) != 0 (mod 5) for k != 5n - 1.
 * https://oeis.org/A224251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(n+1)-2 for n<=4, a(5) = 5^2*2-2, a(n) =
 *            a(n-1)+5*(a(n-4)-a(n-5)) for n>=6 (offset 1)
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 50, u64 n <= 105, u128 n <= 216,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Fast return for n == 0 (unused but harmless) */
    if (n == 0) return (seq_t)0;

    /* Allocate array for values up to n */
    seq_t *arr = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!arr) return (seq_t)0;   /* allocation failure -> 0 */

    /* Base cases */
    if (n >= 1) arr[0] = 5*( (seq_t)2 ) - (seq_t)2;   /* n=1 => 8  */
    if (n >= 2) arr[1] = 5*( (seq_t)3 ) - (seq_t)2;   /* n=2 => 13 */
    if (n >= 3) arr[2] = 5*( (seq_t)4 ) - (seq_t)2;   /* n=3 => 18 */
    if (n >= 4) arr[3] = 5*( (seq_t)5 ) - (seq_t)2;   /* n=4 => 23 */
    if (n >= 5) arr[4] = 5*5*( (seq_t)2 ) - (seq_t)2; /* n=5 => 48 */

    /* Recurrence for n>=6 */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t diff = arr[i-5] - arr[i-6];          /* a(i-4)-a(i-5) */
        seq_t term = 5 * diff;                     /* 5*diff */
        arr[i-1]   = arr[i-2] + term;              /* a(i-1)+5*diff */
    }

    seq_t result = arr[n-1];
    free(arr);
    return result;
}
