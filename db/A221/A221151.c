/*
 * A221151  The generalized Fibonacci word f^[4].
 * https://oeis.org/A221151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 132, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) is the n-th character (0‑based) of the infinite word
 *   S_0 = "0"
 *   S_1 = "0001"
 *   S_k = S_{k-1} S_{k-2}   for k ≥ 2
 * The returned value is 0 or 1.
 */
#include <stddef.h>   /* size_t */
#include <stdint.h>   /* UINT64_MAX */

seq_t a(idx_t n)
{
    /* L[k] = length of S_k.  Store until it would overflow or array is full. */
    static idx_t L[200];
    static size_t Llen = 0;
    static int initialized = 0;

    if (!initialized) {
        L[0] = 1;          /* |S_0| = 1 */
        L[1] = 4;          /* |S_1| = 4 */
        Llen  = 2;
        while (Llen < sizeof(L)/sizeof(L[0])) {
            idx_t a = L[Llen-2];
            idx_t b = L[Llen-1];
            if (b > UINT64_MAX - a) /* next length would overflow */
                break;
            L[Llen++] = a + b;     /* L_k = L_{k-1} + L_{k-2} */
        }
        initialized = 1;
    }

    /* Find the smallest k such that L[k] > n  (i.e. n lies in S_k). */
    size_t k = 0;
    while (k + 1 < Llen && L[k] <= n)
        ++k;

    /* Walk back to the base case by following the concatenation rule. */
    while (k > 1) {
        if (n < L[k-1]) {
            /* first part S_{k-1} */
            --k;
        } else {
            /* second part S_{k-2} */
            n -= L[k-1];
            k -= 2;
        }
    }

    /* Base cases: S_0 = "0" , S_1 = "0001" */
    if (k == 0)
        return (seq_t)0;
    else /* k == 1 */
        return (seq_t)(n == 3 ? 1 : 0);
}
