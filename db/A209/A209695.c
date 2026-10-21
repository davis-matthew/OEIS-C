/*
 * A209695  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209696; see the Formula section.
 * https://oeis.org/A209695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^(k-1) in u(k,x) (k>=1) with offset 1,
 *            where u(1,x)=1, v(1,x)=1, u(n,x)=x*u(n-1,x)+(x+1)*v(n-1,x),
 *            v(n,x)=2x*u(n-1,x)+(x+1)*v(n-1,x)
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code builds the triangle of u‑coefficients incrementally,
   storing each row and answering any a(n) in O(1) after the
   corresponding row has been generated. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"

static seq_t **u_tr = NULL;          /* u‑coefficients rows */
static seq_t **v_tr = NULL;          /* v‑coefficients rows (only for construction) */
static size_t  tr_rows = 0;          /* number of rows already computed */
static size_t  tr_cap  = 0;          /* capacity of the row pointer arrays */

/* Ensure the row pointer arrays have capacity for at least 'cap' rows. */
static void
ensure_capacity(size_t cap)
{
    if (tr_cap >= cap)
        return;
    size_t new_cap = tr_cap ? tr_cap : 2;
    while (new_cap < cap) new_cap <<= 1;
    u_tr = (seq_t**)realloc(u_tr, new_cap * sizeof(seq_t*));
    v_tr = (seq_t**)realloc(v_tr, new_cap * sizeof(seq_t*));
    tr_cap = new_cap;
}

/* Build rows up to and including row 'r' (0‑based). */
static void
make_rows_up_to(size_t r)
{
    if (r < tr_rows)
        return;

    ensure_capacity(r + 1);

    for (size_t i = tr_rows; i <= r; ++i) {
        size_t len = i + 1;                     /* length of the row */
        seq_t *u_row = seq_alloc(len);
        seq_t *v_row = seq_alloc(len);

        if (i == 0) {                          /* n = 1 */
            u_row[0] = 1;
            v_row[0] = 1;
        } else {
            seq_t *prevU = u_tr[i - 1];
            seq_t *prevV = v_tr[i - 1];
            size_t prev_len = i;                /* length of previous row */

            /* k = 0: only the constant term from (x+1)*v */
            u_row[0] = prevV[0];
            v_row[0] = prevV[0];

            for (size_t k = 1; k <= i; ++k) {
                seq_t part1 = (k - 1 < prev_len)  ? prevU[k - 1] : 0;
                seq_t part2 = (k - 1 < prev_len)  ? prevV[k - 1] : 0;
                seq_t part3 = (k < prev_len)      ? prevV[k]     : 0;

                /* u_n = x*u_(n-1) + (x+1)*v_(n-1) */
                u_row[k] = part1 + part2 + part3;

                /* v_n = 2x*u_(n-1) + (x+1)*v_(n-1) */
                v_row[k] = (seq_t)(2 * part1) + part2 + part3;
            }
        }

        u_tr[i] = u_row;
        v_tr[i] = v_row;
    }

    tr_rows = r + 1;
}

/* Return the n-th term of A209695, with offset 1. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; undefined below */
        return 0;

    /* Find the zero‑based row r such that Σ_{j=1}^r j >= n. */
    size_t r = 0;
    while (((r + 1) * (r + 2)) / 2 < n)
        ++r;

    make_rows_up_to(r);

    idx_t base = (idx_t)r * (idx_t)(r + 1) / 2;   /* terms before row r */
    idx_t col  = n - base - 1;                    /* column in row r, 0‑based */

    return u_tr[r][col];
}
