/*
 * A209160  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209161; see the Formula section.
 * https://oeis.org/A209160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle T of coefficients of the polynomials u(n,x) defined by
 *            u(1,x)=1, v(1,x)=1 u(n,x)=u(n-1,x)+(x+1)*v(n-1,x)
 *            v(n,x)=2x*u(n-1,x)+2x*v(n-1,x)+1 a(n) enumerates the entries of
 *            T row‑wise, with a(1)=1 as the first entry (offset 1). The first
 *            rows are 1 2 1 3 6 4
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Allocate an array of seq_t of given length.
   In C++ the elements are default‑constructed; in C no construction is needed. */
static seq_t* alloc_seq(size_t len)
{
#ifdef __cplusplus
    seq_t* p = (seq_t*)malloc(len * sizeof(seq_t));
    if (!p) abort();
    for (size_t i = 0; i < len; ++i)
        new(&p[i]) seq_t();
    return p;
#else
    return (seq_t*)malloc(len * sizeof(seq_t));
#endif
}

/* Return the smallest r such that r*(r+1)/2 >= n. */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Cache of computed rows. */
static seq_t **U_rows = NULL;     /* U_rows[r] -> array of length r */
static idx_t   max_row   = 0;      /* largest r already computed */
static size_t  rows_cap  = 0;      /* capacity of U_rows pointer array */

/* Previous V row (needed to compute the next U and V). */
static seq_t *V_prev   = NULL;      /* V of previous row */
static size_t V_prev_len = 0;       /* length of V_prev */

/* Ensure that rows up to 'need' have been computed. */
static void ensure_rows(idx_t need)
{
    if (need <= max_row)
        return;

    /* Grow U_rows pointer array if necessary. */
    if ((size_t)(need + 1) > rows_cap) {
        size_t new_cap = rows_cap ? rows_cap * 2 : 4;
        while ((size_t)(need + 1) > new_cap)
            new_cap <<= 1;
        seq_t **tmp = (seq_t**)realloc(U_rows, new_cap * sizeof(seq_t*));
        if (!tmp) abort();
        U_rows = tmp;
        rows_cap = new_cap;
    }

    /* Compute rows sequentially. */
    for (idx_t m = max_row + 1; m <= need; ++m) {
        size_t len = (size_t)m;
        seq_t *U_new = alloc_seq(len);

        if (m == 1) {
            /* Base case */
            U_new[0] = 1;
            U_rows[1] = U_new;

            V_prev = alloc_seq(1);
            V_prev[0] = 1;
            V_prev_len = 1;
            continue;
        }

        seq_t *U_prev = U_rows[m - 1];
        seq_t *V_prev_arr = V_prev;   /* old V */

        /* Compute new V row using recurrence:
           V_new[0] = 1
           V_new[k] = 2*(U_prev[k-1] + V_prev_arr[k-1]) for k>=1 */
        seq_t *V_new = alloc_seq(len);
        V_new[0] = 1;
        for (size_t k = 1; k < len; ++k) {
            seq_t u_prev = (k - 1 < m - 1) ? U_prev[k - 1] : (seq_t)0;
            seq_t v_prev = (k - 1 < m - 1) ? V_prev_arr[k - 1] : (seq_t)0;
            V_new[k] = (seq_t)2 * (u_prev + v_prev);
        }

        /* Compute new U row:
           U_new[0] = U_prev[0] + V_prev_arr[0]
           For 1 <= k <= m-2:
             U_new[k] = U_prev[k] + V_prev_arr[k] + V_prev_arr[k-1]
           For k = m-1:
             U_new[k] = V_prev_arr[m-2]  (U_prev[k] and V_prev_arr[k] are 0) */
        U_new[0] = U_prev[0] + V_prev_arr[0];
        for (size_t k = 1; k + 1 < len; ++k) {
            seq_t u_prev = U_prev[k];
            seq_t v_prev_k = V_prev_arr[k];
            seq_t v_prev_km1 = V_prev_arr[k - 1];
            U_new[k] = u_prev + v_prev_k + v_prev_km1;
        }
        if (len >= 2) {
            U_new[len - 1] = V_prev_arr[m - 2];
        }

        U_rows[m] = U_new;

        /* Free old V row and set new one. */
        free(V_prev);
        V_prev = V_new;
        V_prev_len = len;
    }

    max_row = need;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n<1 */

    idx_t r = find_row(n);
    ensure_rows(r);

    idx_t t_prev = (r - 1) * r / 2;          /* triangular number of previous rows */
    idx_t offset = n - t_prev - 1;           /* 0‑based column index */
    return U_rows[r][offset];
}
