/*
 * A136203  Derived Shabat linear tree transform of A053120: Triangle of
 *          coefficients of transformed Chebyshev's T(n, x) polynomials
 *          (powers of x in increasing order) T(x,n)->c*T(c*x+d)+d: c=-1;d=1;
 *          as substitution: 1-x->y( here alternative starting polynomial of
 *          Q(y,1]=1-y.
 * https://oeis.org/A136203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 24, i16 n <= 61, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A136203 – Derived Shabat linear tree transform of A053120
   Coefficients of Q(y,n) where
        Q(0)=1,  Q(1)=1−y
        Q(n)=2y+2(1−y)Q(n−1)−Q(n−2)   (n≥2)
   a(n) is the coefficients of Q(y,r) written row‑wise:
        n=1 ⇒ r=0, coeff 0 = 1
        n=2 ⇒ r=1, coeff 0 = 1
        n=3 ⇒ r=1, coeff 1 = −1
        ...
*/

#include <stdlib.h>
#include <stdint.h>

/* Global storage for the triangle rows */
static seq_t **triangle = NULL;     /* triangle[r] → array of length r+1 */
static size_t capacity = 0;
static size_t size_rows = 0;        /* number of rows already computed */

/* ensure the dynamic array for row pointers has at least 'need' entries */
static void ensure_capacity(size_t need)
{
    if (need < capacity)
        return;
    size_t newcap = capacity ? capacity * 2 : 4;
    while (newcap <= need)
        newcap <<= 1;
    seq_t **tmp = (seq_t **)realloc(triangle, newcap * sizeof(seq_t *));
    if (!tmp)
        abort();
    triangle = tmp;
    capacity = newcap;
}

/* generate rows up to 'target' (inclusive) */
static void ensure_rows(idx_t target)
{
    if ((idx_t)size_rows > target)
        return;
    ensure_capacity((size_t)target + 1);
    for (size_t i = size_rows; i <= (size_t)target; ++i) {
        seq_t *row = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        if (!row)
            abort();
        for (size_t j = 0; j <= i; ++j)
            row[j] = 0;

        if (i == 0) {
            row[0] = 1;
        } else if (i == 1) {
            row[0] = 1;
            row[1] = -1;
        } else {
            seq_t *prev   = triangle[i - 1];
            seq_t *prevpp = triangle[i - 2];

            /* term 1: 2y -> coefficient of y^1 */
            row[1] += 2;

            /* term 2: 2(1−y) * prev */
            for (size_t k = 0; k < i; ++k) {
                seq_t val = prev[k];
                if (k > 0)
                    val -= prev[k - 1];
                row[k] += val * 2;
            }
            /* the last coefficient arising from k = i (−prev[i‑1]) */
            row[i] -= prev[i - 1] * 2;

            /* term 3: −prevpp */
            for (size_t k = 0; k <= i - 2; ++k)
                row[k] -= prevpp[k];
        }
        triangle[i] = row;
    }
    size_rows = target + 1;
}

/* binary search for smallest r with (r+1)(r+2)/2 ≥ n */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    while (( (__int128)(high + 1) * (high + 2) ) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ( ( (__int128)(mid + 1) * (mid + 2) ) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* public function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* only n ≥ 1 are defined */

    idx_t r = find_row(n);
    ensure_rows(r);

    /* number of terms before row r */
    seq_t prev_total = (seq_t)r * ((seq_t)r + 1) / 2;
    seq_t offset_seq = (seq_t)n - prev_total - 1;
    idx_t offset = (idx_t)offset_seq;   /* offset is guaranteed < r+1 */

    return triangle[r][offset];
}
