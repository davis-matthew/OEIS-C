/*
 * A223952  Number of 5 X n 0..1 arrays with diagonals and antidiagonals
 *          unimodal and rows nondecreasing.
 * https://oeis.org/A223952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) for n > 9 with initial values a(1)=32, a(2)=243,
 *            a(3)=596, a(4)=1062, a(5)=1821, a(6)=3115, a(7)=5233, a(8)=8564,
 *            a(9)=13613
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 126, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* Initial values for n = 1 .. 9 */
    static const seq_t base[10] = {
        0ULL,   /* unused index 0 */
        32ULL,  /* a(1) */
        243ULL, /* a(2) */
        596ULL, /* a(3) */
        1062ULL,/* a(4) */
        1821ULL,/* a(5) */
        3115ULL,/* a(6) */
        5233ULL,/* a(7) */
        8564ULL,/* a(8) */
        13613ULL/* a(9) */
    };

    if (n <= 9)
        return base[(idx_t)n];

    /* Allocate array for terms up to n (index 0 unused) */
    idx_t size = n + 1;
    seq_t *arr = (seq_t *)malloc((size_t)size * sizeof(seq_t));
    if (!arr)
        return 0;                /* allocation failure – fallback */

    /* Copy known initial terms */
    for (idx_t i = 1; i <= 9; ++i)
        arr[i] = base[(idx_t)i];

    /* Compute subsequent values using the recurrence */
    for (idx_t i = 10; i <= n; ++i) {
        seq_t val =
            (seq_t)6 * arr[i - 1]
          - (seq_t)15 * arr[i - 2]
          + (seq_t)20 * arr[i - 3]
          - (seq_t)15 * arr[i - 4]
          + (seq_t)6 * arr[i - 5]
          - arr[i - 6];
        arr[i] = val;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
