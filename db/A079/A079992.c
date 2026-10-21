/*
 * A079992  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=3, r=3, I={-2,2}.
 * https://oeis.org/A079992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the linear recurrence a(n) =
 *            a(n-1)+3*a(n-2)-a(n-3)+a(n-4)+a(n-5)+3*a(n-6)
 *            -5*a(n-7)-9*a(n-8)+a(n-9)+a(n-10)+a(n-11)
 *            -a(n-12)+a(n-13)+a(n-14) for n ≥ 14 with initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=9, a(5)=19, a(6)=49, a(7)=100,
 *            a(8)=233, a(9)=503, a(10)=1166, a(11)=2580...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Base values for n=0..13 */
    static const seq_t base[14] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3,
        (seq_t)9, (seq_t)19, (seq_t)49, (seq_t)100,
        (seq_t)233, (seq_t)503, (seq_t)1166, (seq_t)2580,
        (seq_t)5884, (seq_t)13092
    };

    if (n < 14) {
        return base[n];
    }

    /* Allocate array large enough to hold up to n */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr) {
        /* If allocation fails, return 0 (should not happen in tests) */
        return (seq_t)0;
    }

    /* Initialize base entries */
    for (idx_t i = 0; i < 14; ++i) {
        arr[i] = base[i];
    }

    /* Compute recurrence for i = 14 .. n */
    for (idx_t i = 14; i <= n; ++i) {
        /* Sum of positive coefficient terms */
        seq_t pos = arr[i-1]
                  + (seq_t)3 * arr[i-2]
                  + arr[i-4]
                  + arr[i-5]
                  + (seq_t)3 * arr[i-6]
                  + arr[i-9]
                  + arr[i-10]
                  + arr[i-11]
                  + arr[i-13]
                  + arr[i-14];

        /* Sum of negative coefficient terms */
        seq_t neg = arr[i-3]
                  + (seq_t)5 * arr[i-7]
                  + (seq_t)9 * arr[i-8]
                  + arr[i-12];

        arr[i] = pos - neg; /* guaranteed non‑negative */
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
