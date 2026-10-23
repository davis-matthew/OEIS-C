/*
 * A137710  Triangle read by rows: T(n,k) = T(n-1, k-1) - T(n-k, k-1) for k >
 *          1 with T(n,1) = 2^(n-1).
 * https://oeis.org/A137710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   idx_T, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A137710: T(n,k)=T(n-1,k-1)-T(n-k,k-1) for k>1 with T(n,1)=2^(n-1).  
 * Read by rows.  Initial terms: a(1)=1, a(2)=2, a(3)=1, a(4)=4, a(5)=1, a(6)=1, a(7)=8, a(8)=2, a(9)=1, ... */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow_uint.h"
#include "../utils/idx_T.h"

/* internal storage of the already computed triangle */
static seq_t *tri   = NULL;   /* flattened array of triangle entries */
static idx_t tri_cap = 0;     /* capacity of tri in number of entries */
static idx_t max_row  = 0;    /* highest row that has been pre‑computed */

/* grow tri to hold at least new_cap elements */
static void ensure_capacity(idx_t new_cap)
{
    if (new_cap <= tri_cap)
        return;
    size_t new_size = (size_t)new_cap * sizeof(seq_t);
    tri = (seq_t *)realloc(tri, new_size);
    tri_cap = new_cap;
}

/* pre‑compute all rows up to row_needed (inclusive) */
static void ensure_rows(idx_t row_needed)
{
    if (row_needed <= max_row)
        return;

    idx_t new_len = (row_needed * (row_needed + 1)) / 2;
    ensure_capacity(new_len);

    for (idx_t r = max_row + 1; r <= row_needed; ++r) {
        for (idx_t k = 1; k <= r; ++k) {
            seq_t val;
            if (k == 1) {
                /* T(r,1) = 2^(r-1) */
                val = pow_uint((uint64_t)2, r - 1);
            } else {
                seq_t a = tri[idx_T(r - 1, k - 1)];
                seq_t b = 0;
                /* T(r-k,k-1) exists only if r-k >= k-1 (i.e. r >= 2*k-1) */
                if (r >= 2 * k - 1) {
                    b = tri[idx_T(r - k, k - 1)];
                }
                val = a - b;
            }
            tri[idx_T(r, k)] = val;
        }
    }
    max_row = row_needed;
    tri_cap = new_len;  /* now fully occupied */
}

/* find row r so that offset(r) <= n < offset(r+1)
 * offset(r) = r*(r-1)/2 + 1 (1‑based indices) */
static idx_t row_of(idx_t n)
{
    idx_t r = 1;
    while ((r + 1) * r / 2 + 1 <= n)
        ++r;
    return r;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for 0, return 0 */

    idx_t r = row_of(n);
    idx_t k = n - r * (r - 1) / 2;   /* column within the row (1‑based) */

    ensure_rows(r);
    return tri[idx_T(r, k)];
}
