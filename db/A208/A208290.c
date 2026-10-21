/*
 * A208290  Number of 6 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A208290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 11*a(n-2) - 10*a(n-3) - 10*a(n-4) + 6*a(n-5) +
 *            2*a(n-6) - a(n-7) Initial terms (n = 1 … 7): a(1)=12, a(2)=144,
 *            a(3)=636, a(4)=3900, a(5)=21096, a(6)=119580, a(7)=665892
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init[7] = {
    (seq_t)12,
    (seq_t)144,
    (seq_t)636,
    (seq_t)3900,
    (seq_t)21096,
    (seq_t)119580,
    (seq_t)665892
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe */
    if (n <= 7) return _init[n-1];

    size_t len = (size_t)n;
    seq_t *arr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!arr) return (seq_t)0;           /* allocation failure */

    /* copy the initial 7 terms */
    for (size_t i = 0; i < 7; ++i) arr[i] = _init[i];

    /* compute using the recurrence */
    for (idx_t i = 7; i < n; ++i) {
        seq_t pos = (seq_t)4 * arr[i-1]
                  + (seq_t)11 * arr[i-2]
                  + (seq_t)6  * arr[i-5]
                  + (seq_t)2  * arr[i-6];

        seq_t neg = (seq_t)10 * arr[i-3]
                  + (seq_t)10 * arr[i-4]
                  + arr[i-7];

        arr[i] = pos - neg;
    }

    seq_t result = arr[n-1];
    free(arr);
    return result;
}
