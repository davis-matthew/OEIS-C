/*
 * A211793  Rectangular array: R(k,n) = number of ordered triples (w,x,y) with
 *          all terms in {1,...,n} and w^k >= x^k + y^k.
 * https://oeis.org/A211793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the rectangular array R(k,m) =
 *            #{(w,x,y)∈[1..m]^3 : w^k ≥ x^k + y^k} in anti‑diagonal order of
 *            the table R(k,m). That is, a(1)=R(1,1), a(2)=R(1,2),
 *            a(3)=R(2,1), a(4)=R(1,3), …
 * Range:     u8 n <= 24, u16 n <= 60, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   count_R, powers_of_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/count_R.h"
#include "../utils/powers_of_idx.h"

/* ------------------------------------------------------------ */
/* Helper: determine the diagonal (sum k+m) that contains index n
   The table contains one entry for every pair (k,m) with k≥1, m≥1, and
   the pairs are listed by increasing k+m.  For a given n (1‑based)
   find the smallest integer s≥2 such that (s-1)*s/2 ≥ n.   */
static idx_t diag_of(idx_t idx)
{
    idx_t lo = 2, hi = 2000;                 /* hi large enough for all n used */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ( (mid - 1) * mid / 2 >= idx )
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* Public entry point required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1 (a(1) is first) */

    /* Determine diagonal s and offset inside that diagonal */
    idx_t s = diag_of(n);                       /* s ≥ 2, (s-1)*s/2 ≥ n */
    idx_t prev = (s - 2) * (s - 1) / 2;          /* entries before diag s */
    idx_t offset = n - prev;                     /* 1‑based offset in diag */

    idx_t k = offset;            /* k = offset */
    idx_t n_val = s - offset;    /* n' = s – offset */

    /* Safety check for excessively large inputs */
    if (k == 0 || n_val == 0) return 0;

    /* Build powers and count R(k,n_val) */
    seq_t *p = powers_of_idx(k, n_val);
    if (!p) return 0;              /* allocation failed: treat as 0 */

    seq_t result = count_R(k, n_val, p);
    free(p);
    return result;
}
