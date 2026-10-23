/*
 * A140224  Triangle: T(n,m) is number of m's among (d(1),d(2),...,d(n)),
 *          where d(n) = A000005(n).
 * https://oeis.org/A140224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(k, m) where k is the smallest integer with k(k+1)/2 ≥
 *            n, m = n - (k-1)k/2. Then a(n) is the number of integers j ≤ k
 *            with d(j) = m, i.e. the (k,m)th entry of the OEIS triangle
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* Find the row k and column m corresponding to the linear index n. */
    idx_t k = 0;
    idx_t sum = 0;                 /* sum = 1 + 2 + ... + k        */
    while (sum < n) {
        ++k;
        sum += k;
    }
    idx_t m = n - (sum - k);       /* column index (1‑based)        */

    /* Compute divisor counts d[1..k] */
    size_t sz = (size_t)(k + 1);
    seq_t *d = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!d) return 0;              /* allocation failed */
    for (size_t i = 1; i <= k; ++i) d[i] = 0;

    for (size_t i = 1; i <= k; ++i) {
        for (size_t j = i; j <= k; j += i) {
            d[j] += 1;
        }
    }

    /* Count how many j ≤ k have d[j] == m */
    seq_t cnt = 0;
    for (size_t i = 1; i <= k; ++i) {
        if (d[i] == (seq_t) m)
            ++cnt;
    }

    free(d);
    return cnt;
}
