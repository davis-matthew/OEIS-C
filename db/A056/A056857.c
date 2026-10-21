/*
 * A056857  Triangle read by rows: T(n,c) = number of successive equalities in
 *          set partitions of n.
 * https://oeis.org/A056857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl, nice
 * Formula:   a(n) = B(r‑1‑c) * C(r‑1, c) where (r,c) are the row and column
 *            of the n‑th element of the triangle T(n,c)=Bell(n‑1‑c)*C(n‑1,c)
 *            read row‑wise
 * Range:     u8 n <= 22, u16 n <= 55, u32 n <= 136, u64 n <= 351,
 *            u128 n <= 946, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/binomial.h"

static seq_t bell[200];
static idx_t bell_size = 0;
static bool bell_init = false;

static void ensure_bell(idx_t need)
{
    if (!bell_init) {
        bell[0] = 1;
        bell_size = 1;
        bell_init = true;
    }
    while (bell_size <= need) {
        idx_t m = bell_size;                      /* compute B(m) */
        seq_t sum = 0;
        for (idx_t k = 0; k < bell_size; ++k) {   /* B(m) = Σ C(m‑1,k)·B(k) */
            sum += binomial(m - 1, k) * bell[k];
        }
        bell[bell_size++] = sum;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for offset 0 */

    /* Find row r such that r(r‑1)/2 < n ≤ r(r+1)/2 */
    idx_t r = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Column index within the row (0‑based) */
    idx_t col = n - (r * (r - 1) / 2) - 1;

    /* Bell index needed */
    idx_t bell_idx = r - 1 - col; /* B(r‑1‑c) */
    ensure_bell(bell_idx);

    seq_t bin = binomial(r - 1, col);
    return bell[bell_idx] * bin;
}
