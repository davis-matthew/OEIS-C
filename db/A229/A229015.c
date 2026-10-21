/*
 * A229015  Number of arrays of median of three adjacent elements of some
 *          length 7 0..n array, with no adjacent equal elements in the
 *          latter.
 * https://oeis.org/A229015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) for n>6, with initial values a(1)=2, a(2)=57, a(3)=332,
 *            a(4)=1145, a(5)=3002, a(6)=6635
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 104, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_seq[7] = {0, 2, 57, 332, 1145, 3002, 6635};

seq_t a(idx_t n)
{
    if (n <= 6)
        return base_seq[n];
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr)
        return 0;
    arr[0] = 0;
    arr[1] = base_seq[1];
    arr[2] = base_seq[2];
    arr[3] = base_seq[3];
    arr[4] = base_seq[4];
    arr[5] = base_seq[5];
    arr[6] = base_seq[6];
    for (idx_t i = 7; i <= n; ++i)
    {
        seq_t pos = 6 * arr[i-1] + 20 * arr[i-3] + 6 * arr[i-5];
        seq_t neg = 15 * arr[i-2] + 15 * arr[i-4] + arr[i-6];
        arr[i] = pos - neg;
    }
    seq_t res = arr[n];
    free(arr);
    return res;
}
