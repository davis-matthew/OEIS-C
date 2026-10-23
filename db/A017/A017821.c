/*
 * A017821  Expansion of 1/(1-x^3-x^4-x^5-x^6-x^7-x^8).
 * https://oeis.org/A017821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8) for n>7,
 *            a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=1, a(6)=2, a(7)=3
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 66, u64 n <= 127,
 *            u128 n <= 251, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)0;

    seq_t *arr = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!arr) return (seq_t)0;  /* allocation failure: return 0 (unlikely) */

    arr[0] = (seq_t)1;
    if (n >= 1) arr[1] = (seq_t)0;
    if (n >= 2) arr[2] = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t sum = 0;
        if (i >= 3) sum += arr[i - 3];
        if (i >= 4) sum += arr[i - 4];
        if (i >= 5) sum += arr[i - 5];
        if (i >= 6) sum += arr[i - 6];
        if (i >= 7) sum += arr[i - 7];
        if (i >= 8) sum += arr[i - 8];
        arr[i] = sum;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
