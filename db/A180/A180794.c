/*
 * A180794  Number of distinct solutions of sum{i=1..2}(x(2i-1)*x(2i)) = 0
 *          (mod n), with x() in 0..n-1.
 * https://oeis.org/A180794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{0 <= r < n} * if r < (n-r)%n: g[r] * g[(n-r)%n] *
 *            else if r == (n-r)%n: g[r] * (g[r]+1) / 2 * where g[r] is the
 *            number of unordered pairs (a,b) with 0 <= a <= b < n * such that
 *            a*b ≡ r (mod n). The function counts unordered pairs of *
 *            unordered pairs meeting the congruence sum{x1*x2 +...
 * Range:     u8 n <= 11, u16 n <= 69, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle trivial case */
    if (n == 0) return 0;

    /* Allocate array for counts of unordered pairs per residue */
    seq_t *g = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!g) return 0; /* allocation failure */

    /* Initialize to zero */
    for (idx_t i = 0; i < n; ++i) g[i] = 0;

    /* Count unordered pairs (a,b) with a <= b */
    for (idx_t a = 0; a < n; ++a) {
        for (idx_t b = a; b < n; ++b) {
            idx_t prod = (a * b) % n;
            g[prod] += 1;
        }
    }

    /* Compute the result */
    seq_t total = 0;
    for (idx_t r = 0; r < n; ++r) {
        idx_t rr = (n - r) % n;
        if (r < rr) {
            total += g[r] * g[rr];
        } else if (r == rr) {
            /* combinations with replacement */
            total += g[r] * (g[r] + 1) / 2;
        }
        /* when r > rr, already counted */
    }

    free(g);
    return total;
}
