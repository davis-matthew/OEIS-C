/*
 * A059484  Triangle T(n,k) = coefficient of x^n*y^k in
 *          1/(1-x*y-x*y^2-x^2*y^3), read by rows in order 00, 10, 01, 20, 11,
 *          02, ...
 * https://oeis.org/A059484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the coefficient of x^n y^k in 1/(1-x*y-x*y^2-x^2*y^3),
 *            listed in anti‑diagonal order 00,10,01,20,11,02,…
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   antidiagonal_coords, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recurrence  
      T(0,0)=1; T(i,j)=T(i-1,j-1)+T(i-1,j-2)+T(i-2,j-3)  (indices <0 are 0).  
   a(n)=T(i,j) where (i,j) are the anti‑diagonal coordinates of n.  */

#include <stddef.h>
#include "../utils/antidiagonal_coords.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *tbl = NULL;      /* flat 2‑D table: tbl[i*sz + j] = T(i,j) */
static size_t sz  = 0;          /* current side length of tbl */

/* local helper to ensure tbl is large enough for indices up to i,j */
static void ensure_table(idx_t i, idx_t j)
{
    idx_t needed = (i > j) ? i : j;
    idx_t new_sz = needed + 1;
    if ((size_t)new_sz <= sz)
        return;                /* already big enough */

    /* allocate a fresh table */
    seq_t *new_tbl = seq_alloc_new_array(new_sz * new_sz);
    if (!new_tbl)
        return;                /* allocation failed – caller will get 0 */

    /* fill the table using the recurrence */
    for (idx_t ii = 0; ii < new_sz; ++ii) {
        for (idx_t jj = 0; jj < new_sz; ++jj) {
            seq_t val = 0;
            if (ii == 0 && jj == 0) {
                val = 1;
            } else {
                if (ii >= 1 && jj >= 1)
                    val += new_tbl[(size_t)(ii - 1) * new_sz + (size_t)(jj - 1)];
                if (ii >= 1 && jj >= 2)
                    val += new_tbl[(size_t)(ii - 1) * new_sz + (size_t)(jj - 2)];
                if (ii >= 2 && jj >= 3)
                    val += new_tbl[(size_t)(ii - 2) * new_sz + (size_t)(jj - 3)];
            }
            new_tbl[(size_t)ii * new_sz + (size_t)jj] = val;
        }
    }

    /* replace old table */
    if (tbl)
        seq_free_array(tbl);
    tbl = new_tbl;
    sz  = (size_t)new_sz;
}

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);     /* utility returns (j,i); swap */
    idx_t ii = j;    /* original i coordinate */
    idx_t jj = i;    /* original j coordinate */

    ensure_table(ii, jj);

    /* now tbl has enough space */
    return tbl[(size_t)ii * sz + (size_t)jj];
}
