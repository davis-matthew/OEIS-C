/*
 * A116826  Number of permutations of length n which avoid the patterns 2143,
 *          2431, 3124.
 * https://oeis.org/A116826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 25*a(n-2) + 41*a(n-3) - 38*a(n-4) + 19*a(n-5)
 *            - 4*a(n-6); Initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=21,
 *            a(5)=73, a(6)=244
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* unused, offset starts at 1 */

    /* Initial values for n = 1 … 6 */
    const seq_t init[6] = {1, 2, 6, 21, 73, 244};

    if (n <= 6) return init[n - 1];

    /* Keep the last six values in a circular buffer: arr[0..5] corresponds
       to a(n-6) … a(n-1).  Initially arr[0..5] = a(1)…a(6).  After each
       step, we compute a(n) and shift the window. */
    seq_t arr[6];
    for (idx_t i = 0; i < 6; ++i) arr[i] = init[i];

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute a(i) = 8*arr[5] - 25*arr[4] + 41*arr[3] - 38*arr[2] + 19*arr[1] - 4*arr[0] */
        seq_t pos = arr[5] * 8;
        pos += arr[3] * 41;
        pos += arr[1] * 19;

        seq_t neg = arr[4] * 25;
        neg += arr[2] * 38;
        neg += arr[0] * 4;

        seq_t next = pos - neg;

        /* Shift window */
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = arr[3];
        arr[3] = arr[4];
        arr[4] = arr[5];
        arr[5] = next;
    }

    return arr[5];
}
