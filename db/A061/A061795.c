/*
 * A061795  Number of distinct sums phi(i) + phi(j) for 1 <= i <= j <= n,
 *          phi(k) = A000010(k).
 * https://oeis.org/A061795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct values of φ(i)+φ(j) for 1 ≤ i ≤ j ≤ n,
 *            where φ(k) is Euler's totient function
 * Range:     u8 n <= 137, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is computed by:
 *  1. Computing φ(1)…φ(n) by a standard sieve.
 *  2. Marking all sums φ(i)+φ(j) (i ≤ j) in a boolean array of size 2*n+1.
 *  3. Counting how many positions in the array were set.
 *  4. Returning that count as seq_t.
 *
 *  This runs in O(n^2) time and uses O(n) memory, fast enough for n up to 10000. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for n < 1, return 0 */

    /* --- Compute totient values φ(1)…φ(n) --- */
    seq_t *phi = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!phi)
        return 0;

    for (idx_t i = 0; i <= n; ++i)
        phi[i] = (seq_t)i;             /* start with φ(i)=i */

    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == (seq_t)p) {      /* p is prime */
            for (idx_t k = p; k <= n; k += p)
                phi[k] -= phi[k] / (seq_t)p;
        }
    }
    /* φ(1) is already 1 */

    /* --- Mark distinct sums φ(i)+φ(j) --- */
    size_t maxSum = 2 * (size_t)n;          /* φ(i) ≤ i, so sum ≤ 2n */
    char *mark = (char *)calloc(maxSum + 1, 1);
    if (!mark) {
        free(phi);
        return 0;
    }

    seq_t distinct = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t phi_i = phi[i];
        for (idx_t j = i; j <= n; ++j) {
            seq_t sum = phi_i + phi[j];
            size_t idx = (size_t)sum;      /* sum fits in size_t (≤ 2n) */
            if (!mark[idx]) {
                mark[idx] = 1;
                distinct++;
            }
        }
    }

    free(phi);
    free(mark);
    return distinct;
}
