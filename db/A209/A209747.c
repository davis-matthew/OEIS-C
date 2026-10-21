/*
 * A209747  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209748; see the Formula section.
 * https://oeis.org/A209747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients of polynomials u(n,x).  a(n) is the nth
 * coefficient read row‑by‑row.  Recurrence:
 *   u(n,x) = x*u(n-1,x) + (x+1)*v(n-1,x)
 *   v(n,x) = u(n-1,x) + v(n-1,x) + 1
 * with u(1,x)=1, v(1,x)=1.  */
#include <stdlib.h>

/* Find the row r (r>=1) such that the total number of terms up to that
 * row, r*(r+1)/2, is at least n.  */
static idx_t
find_row(idx_t n)
{
    if (n <= 1) return 1;
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Static storage of the current triangle row.  U holds the
 * coefficients of u(r,x), V holds the coefficients of v(r,x)
 * padded with an extra zero at the end for ease of access.  */
static seq_t *U = NULL;   /* coefficients of u(r,x) */
static seq_t *V = NULL;   /* coefficients of v(r,x) (with trailing zero) */
static idx_t cur_len = 0; /* number of coefficients currently stored (row r) */

/* Compute the a(n) term. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined below the offset */

    /* Determine the row that contains the n-th term. */
    idx_t r = find_row(n);

    /* Build rows up to r if not already done. */
    while (cur_len < r) {
        size_t old_len = (size_t)cur_len;      /* previous row length = r-1 */
        size_t new_len = old_len + 1;          /* new row length = r */

        /* Allocate space for the new row. */
        seq_t *U_new = (seq_t *)malloc(new_len * sizeof(seq_t));
        seq_t *V_new = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!U_new || !V_new) {
            free(U_new); free(V_new);
            return (seq_t)0;   /* allocation failure */
        }

        /* If we are building the very first row, initialize it directly. */
        if (cur_len == 0) {
            U_new[0] = (seq_t)1;
            V_new[0] = (seq_t)1;
            U_new[0] = (seq_t)1;           /* u(1,x)=1 */
            V_new[0] = (seq_t)1;           /* v(1,x)=1 */
            V_new[0] = (seq_t)1;           /* redundant, kept for clarity */
            V_new[0] = (seq_t)1;
            V_new[0] = (seq_t)1;
            V_new[0] = (seq_t)1;
            /* The trailing zero is already zero by malloc. */
        } else {
            /* Compute coefficients of u(r,x) and v(r,x). */
            for (size_t k = 0; k < new_len; ++k) {
                seq_t vprev_k   = (k < old_len) ? V[k] : (seq_t)0;
                seq_t vprev_k1  = (k > 0) ? V[k - 1] : (seq_t)0;
                seq_t uprev_k1  = (k > 0) ? U[k - 1] : (seq_t)0;

                /* u_r[k] = (k>0 ? Uprev[k-1] + Vprev[k-1] : 0) + Vprev[k] */
                if (k > 0)
                    U_new[k] = uprev_k1 + vprev_k1 + vprev_k;
                else
                    U_new[k] = vprev_k;
            }

            /* v_r[0] = U[0] + V[0] + 1,  v_r[k] = U[k] + V[k] for k>=1 */
            V_new[0] = U[0] + V[0] + (seq_t)1;
            for (size_t k = 1; k < old_len; ++k)
                V_new[k] = U[k] + V[k];
            /* The last entry V_new[new_len-1] remains zero (malloced to zero) */
        }

        /* Release old arrays and update static pointers. */
        free(U);
        free(V);
        U = U_new;
        V = V_new;
        cur_len = (idx_t)new_len;
    }

    /* Compute the position of the coefficient within the row. */
    idx_t prev_sum = (r - 1) * r / 2;                    /* terms before row r */
    idx_t pos = n - prev_sum - (idx_t)1;                 /* 0‑based index in row r */

    return U[(size_t)pos];
}
