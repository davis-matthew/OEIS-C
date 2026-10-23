/*
 * A121400  Triangle, read by rows, where T(n,k) = T(n-1,k-1) + T(n-1,k) +
 *          T(n-1,k+1) for n>=k>=1, with T(0,0) = 1, T(n,n) = T(n,0) +
 *          T(n-1,n-1) for n>=1; T(n,k)=0 when n<k or k<0.
 * https://oeis.org/A121400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A121400: triangle read by rows.  
   The triangle T(n,k) is defined by  
      T(0,0)=1, 
      T(n,k)=T(n-1,k-1)+T(n-1,k)+T(n-1,k+1)   for 1≤k≤n-1,  
      T(n,n)=T(n,0)+T(n-1,n-1)                for n≥1,  
      T(n,k)=0 for k<0 or k>n.  
   Sequence a(n) is obtained by reading the triangle row‑wise (row 0 has
   one element, row 1 has two, etc.).  That is, for index n we locate
   its row r and column c :
      r = floor((sqrt(8·n+1)-1)/2)          // r starts at 0
      c = n - r·(r+1)/2                     // offset within row
   and a(n)=T(r,c).
   The implementation builds rows on demand, caching all previous rows
   in a static array.  Only integer arithmetic on indices and the
   supplied integer type seq_t is used; the triangle is constructed
   by the recurrence above.  The routine works for any seq_t (arbitrary
   precision or fixed unsigned width) and any idx_t (uint64_t).  
*/
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n) {
    /* static cache of triangle rows */
    static seq_t **rows = NULL;      /* array of pointers to rows */
    static idx_t  rows_count = 0;     /* number of rows stored (max index+1) */

    /* determine which row contains index n */
    double t = sqrt(8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((t - 1.0) / 2.0);          /* row index, 0‑based */

    /* grow cache until we have rows[0..r] */
    if (rows_count <= r) {
        /* allocate or enlarge the row pointer array */
        idx_t new_cap = (rows_count == 0) ? 1 : rows_count * 2;
        while (new_cap <= r) new_cap *= 2;
        seq_t **tmp = (seq_t **)realloc(rows, sizeof(seq_t *) * new_cap);
        rows = tmp;
        rows_count = new_cap;
    }

    /* create new rows as needed */
    for (idx_t i = 0; i <= r; ++i) {
        if (i < r + 1 && rows[i] != NULL) continue;  /* already computed */

        /* allocate space for row i (size i+1) */
        seq_t *row = (seq_t *)malloc(sizeof(seq_t) * (i + 1));
        if (!row) exit(1);  /* abort on bad alloc */

        if (i == 0) {
            row[0] = (seq_t)1;             /* base case */
        } else {
            seq_t *prev = rows[i - 1];
            /* compute all columns except the last */
            for (idx_t k = 0; k < i; ++k) {
                seq_t a = (k > 0)          ? prev[k - 1] : (seq_t)0;
                seq_t b = prev[k];
                seq_t c = (k + 1 <= i - 1) ? prev[k + 1] : (seq_t)0;
                row[k] = a + b + c;
            }
            /* last element uses the special recurrence */
            row[i] = row[0] + prev[i - 1];
        }
        rows[i] = row;
    }

    /* locate column within row r */
    idx_t col = n - r * (r + 1) / 2;
    return rows[r][col];
}
