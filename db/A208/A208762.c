/*
 * A208762  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208761; see the Formula section.
 * https://oeis.org/A208762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the triangle T(n,k):
   T(0,0) = 1,
   T(1,0) = T(1,1) = 2,
   T(n,k) = 2*T(n-1,k) + T(n-1,k-1) - T(n-2,k)
             + T(n-2,k-1) + 2*T(n-2,k-2)   for n ≥ 2, 0 ≤ k ≤ n.
   The sequence a(n) is the triangle flattened row‑by‑row:
   a(1)=T(0,0), a(2)=T(1,0), a(3)=T(1,1), a(4)=T(2,0), …      */

#include <stdlib.h>

/* Integer square‑root: floor(sqrt(x)) for 0≤x<2^64 */
static idx_t floor_isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = ((idx_t)1ULL << 62);          /* highest power of 4 ≤ 2^64 */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Triangle storage */
static seq_t **tri = NULL;          /* triangle rows 0 … */
static size_t tri_rows_cap = 0;     /* allocated capacity of tri */
static size_t tri_rows_max = 0;     /* number of rows currently computed (next free index) */

/* Compute rows 0 … up_to inclusive and store them in tri. */
static void ensure_rows_to(size_t up_to)
{
    if (tri_rows_max >= up_to + 1) return;          /* already enough */

    /* grow array of pointers if necessary */
    size_t new_cap = tri_rows_cap;
    if (new_cap == 0) new_cap = 2;
    while (new_cap < up_to + 1) new_cap <<= 1;
    if (new_cap > tri_rows_cap) {
        tri = (seq_t**)realloc(tri, new_cap * sizeof(seq_t*));
        tri_rows_cap = new_cap;
    }

    /* compute missing rows */
    for (size_t i = tri_rows_max; i <= up_to; ++i) {
        size_t len = i + 1;
        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
        if (!row) return;   /* allocation failure – sequence cannot be produced */

        if (i == 0) {
            row[0] = 1;
        } else if (i == 1) {
            row[0] = row[1] = 2;
        } else {
            seq_t *prev1 = tri[i-1];
            seq_t *prev2 = tri[i-2];
            for (size_t k = 0; k <= i; ++k) {
                seq_t val = 0;
                if (k < i)          val += 2 * prev1[k];
                if (k > 0)          val += prev1[k-1];
                if (k < i-1)        val -= prev2[k];
                if (k > 0 && k-1 < i-1) val += prev2[k-1];
                if (k > 1)          val += 2 * prev2[k-2];
                row[k] = val;
            }
        }
        tri[i] = row;
    }
    tri_rows_max = up_to + 1;
}

/* Public function computing the nth term (1‑based) of the flattened triangle. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* below offset – undefined behaviour */

    idx_t N = n;
    /* solve r such that (r+1)(r+2)/2 ≥ N  (r is 0‑based row index) */
    idx_t d = floor_isqrt_u64(8 * N + 1);
    size_t r = (size_t)((d - 3) / 2);   /* floor of the solution */
    while (((seq_t)(r + 1) * (seq_t)(r + 2) / 2) < (seq_t)N) ++r;

    /* ensure the triangle is computed up to row r */
    ensure_rows_to(r);

    seq_t prev_total = (seq_t)r * (seq_t)(r + 1) / 2;   /* terms before row r */
    size_t k = (size_t)(N - prev_total - 1);           /* index within the row */

    return tri[r][k];
}
