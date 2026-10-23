/*
 * A117905  Expansion of (1+2*x+2*x^2)/((1+x)*(1-x^3)^2).
 * https://oeis.org/A117905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0..6) = 1, 1, 1, 1, 3, 1, 2 a(n) = -a(n-1) + 2*a(n-3) +
 *            2*a(n-4) - a(n-6) - a(n-7) (n ≥ 7)
 * Range:     u8 n <= 510, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small indices are returned directly from the known seed values. */
    if (n < 7) {
        static const seq_t base[7] = {1, 1, 1, 1, 3, 1, 2};
        return base[(size_t)n];
    }

    /* Allocate a temporary array of length n+1 and compute the sequence
       iteratively.  This keeps the code simple and guarantees correctness
       for all n, even when the calls are in arbitrary order. */
    size_t len   = (size_t)n + 1;
    seq_t *arr   = (seq_t *)malloc(len * sizeof(seq_t));
    if (!arr) return 0;               /* allocation failure – return 0 */

    /* Initialise the first seven terms. */
    static const seq_t base[7] = {1, 1, 1, 1, 3, 1, 2};
    for (size_t i = 0; i < 7; ++i) arr[i] = base[i];

    /* Recurrence: a(i) = 2*a(i-3) + 2*a(i-4) - (a(i-1)+a(i-6)+a(i-7))   */
    for (size_t i = 7; i < len; ++i) {
        seq_t sum1 = arr[i - 3] * 2 + arr[i - 4] * 2;
        seq_t sum2 = arr[i - 1] + arr[i - 6] + arr[i - 7];
        arr[i] = sum1 - sum2;          /* sum1 ≥ sum2 for every n ≥ 7 */
    }

    seq_t result = arr[len - 1];
    free(arr);
    return result;
}
