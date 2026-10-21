/*
 * A209232  a(n) is 2^n times the expected value of the shortest run of 0's in
 *          a binary word of length n.
 * https://oeis.org/A209232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A209232:  a(n) is 2^n times the expected value of the shortest run
   of 0's in a binary word of length n.
   Formula used:  a(n) = sum_{k=1..n} f_k(n) – n,
   where f_k(n) is the number of binary words of length n whose every run of 0's
   has length at least k.  f_k(n) is obtained by dynamic programming:

   Let A[i]   – number of valid words of length i that end with 1
   Let B[i]   – number of valid words of length i that end with a run of 0's
                of length at least k
   Then for i ≥ 1
        if i < k :  A[i] = 1 , B[i] = 0
        else      :  A[i] = A[i-1] + B[i-1]
                     B[i] = B[i-1] + A[i-k]
   The desired count is f_k(n) = A[n] + B[n].
   The implementation computes f_k(n) for every k = 1..n in O(n) time each,
   resulting in O(n²) time for a(n) and O(n) auxiliary memory. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* temporary array for the A values, reused for each k */
    seq_t *A = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!A) return 0;  /* allocation failed – returning 0 as fallback */

    seq_t sum = 0;

    for (int k = 1; k <= (int)n; ++k) {
        seq_t Bprev = 0;          /* B[i-1] for current i   */
        A[0] = 1;                 /* empty word ends with 1 */

        for (int i = 1; i <= (int)n; ++i) {
            seq_t Ai, Bi;
            if (i < k) {
                Ai = 1;
                Bi = 0;
            } else {
                Ai = A[i-1] + Bprev;
                Bi = Bprev + A[i-k];
            }
            A[i] = Ai;
            Bprev = Bi;
        }

        sum += A[n] + Bprev;      /* f_k(n) = A[n] + B[n] */
    }

    free(A);
    return sum - (seq_t)n;        /* subtract the n words with no 0's   */
}
