/*
 * A028376  Triangle read by rows: T(n,m) = Sum Catalan(n-k)*Catalan(k),
 *          k=0..m.
 * https://oeis.org/A028376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   catalan, triangular_rows
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028376:  Triangle read by rows
 * T(n,m) = Sum_{k=1..m} Catalan(n-k)*Catalan(k-1)
 * 0‑based offset; a(n) is the flattened triangle read row‑wise.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/catalan.h"
#include "../utils/triangular_rows.h"

/*
 * a(n) gives the n-th term of OEIS A028376.
 * The function uses only integer arithmetic and the provided
 * catalan() helper.  All arithmetic is performed in seq_t, the
 * target integer type supplied by the build.
 */
seq_t a(idx_t n) {
    if (n == 0) /* first term listed is 0 */
        return 0;

    idx_t r = triangular_rows(n);  /* row index (0‑based)   */
    idx_t m = n - r * (r + 1) / 2; /* column index (0‑based) */

    seq_t sum = 0;
    /* sum for k = 1 .. m of Catalan(r‑k) * Catalan(k‑1) */
    for (idx_t k = 1; k <= m; ++k) {
        sum += catalan(r - k) * catalan(k - 1);
    }
    return sum;
}
