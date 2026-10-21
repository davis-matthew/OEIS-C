/*
 * A234134  Number of (n+1) X (2+1) 0..3 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of all six edge and diagonal
 *          differences equal to 9.
 * https://oeis.org/A234134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A234134 – Number of (n+1)×(2+1) 0..3 arrays with every 2×2 subblock
 * having the sum of the absolute values of the six edge and diagonal differences
 * equal to 9.
 *
 * Empirical recurrence (holds for n ≥ 7):
 *   a(n) = 3*a(n-1) + 8*a(n-2) - 30*a(n-3) + 4*a(n-4)
 *        + 48*a(n-5) - 32*a(n-6)
 *
 * Initial terms (1 ≤ n ≤ 6):
 *   a(1)=80, a(2)=152, a(3)=296, a(4)=680,
 *   a(5)=1544,  a(6)=4040.
 *
 * The function below computes a(n) for any n ≥ 1 using the above recurrence.
 * It allocates a temporary array of size n+1 for the computation and frees it
 * before returning, thus avoiding any static state and guaranteeing correct
 * operation irrespective of the order in which a() is called.
 */

#include <stdlib.h>

/* Compute the nth term of A234134.  The index is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* Allocate array of (n+1) elements; index 0 is unused. */
    /* Use malloc rather than calloc – we know all entries will be set. */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr)
        abort();           /* allocation failed */

    /* Base cases for n = 1 … 6. */
    if (n >= 1) arr[1] = (seq_t)80;
    if (n >= 2) arr[2] = (seq_t)152;
    if (n >= 3) arr[3] = (seq_t)296;
    if (n >= 4) arr[4] = (seq_t)680;
    if (n >= 5) arr[5] = (seq_t)1544;
    if (n >= 6) arr[6] = (seq_t)4040;

    /* Use the empirical recurrence for n ≥ 7. */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t t = arr[i-1] * (seq_t)3
                + arr[i-2] * (seq_t)8
                - arr[i-3] * (seq_t)30
                + arr[i-4] * (seq_t)4
                + arr[i-5] * (seq_t)48
                - arr[i-6] * (seq_t)32;
        arr[i] = t;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
