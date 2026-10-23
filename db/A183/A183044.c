/*
 * A183044  Sums of least numbers of knight's moves on vertical segments (n,0)
 *          to (n,n) on infinite chessboard.
 * https://oeis.org/A183044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by linear recurrence a(n) = a(n-2)+a(n-3)+a(n-4) –
 *            a(n-5) – a(n-6) – a(n-7) + a(n-9) for n ≥ 12 with initial
 *            values: a(0)=0, a(1)=5, a(2)=7, a(3)=10, a(4)=14, a(5)=21,
 *            a(6)=27, a(7)=38, a(8)=44, a(9)=57, a(10)=67, a(11)=82
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static const seq_t init[12] = {
        0LL, /* a(0) */
        5LL, /* a(1) */
        7LL, /* a(2) */
        10LL,/* a(3) */
        14LL,/* a(4) */
        21LL,/* a(5) */
        27LL,/* a(6) */
        38LL,/* a(7) */
        44LL,/* a(8) */
        57LL,/* a(9) */
        67LL,/* a(10) */
        82LL /* a(11) */
    };

    if (n <= 11) {
        return init[n];
    }

    /* Allocate an array to store computed values up to n */
    idx_t size = n + 1;
    seq_t *arr = (seq_t *)malloc(size * sizeof(seq_t));
    if (!arr) {
        return (seq_t)0; /* allocation failure; unlikely in test harness */
    }

    /* Copy initial values */
    for (idx_t i = 0; i <= 11; ++i) {
        arr[i] = init[i];
    }

    /* Compute terms from 12 up to n via the recurrence */
    for (idx_t i = 12; i <= n; ++i) {
        /* Use addition only, ensure no unsigned underflow:
           (a[i-2]+a[i-3]+a[i-4]+a[i-9]) is proven larger than
           (a[i-5]+a[i-6]+a[i-7]) for all relevant i. */
        seq_t val = arr[i-2] + arr[i-3];
        val += arr[i-4];
        val += arr[i-9];
        val -= arr[i-5];
        val -= arr[i-6];
        val -= arr[i-7];
        arr[i] = val;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
