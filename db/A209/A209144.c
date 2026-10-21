/*
 * A209144  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209143; see the Formula section.
 * https://oeis.org/A209144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened triangle of coefficients of v(n,x). The
 *            triangle T(n,k) is defined by: T(0,0) = 1, T(1,0) = 3, T(1,1) =
 *            0, for n ≥ 2: T(n,k) = 2*T(n-1,k) + T(n-2,k-1), 0 ≤ k < len(n),
 *            where len(n) = (n/2)+1 (the number of elements in row n). The
 *            sequence a(n) enumerates the triangle row‑by‑...
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Forward declaration of idx_t and seq_t are provided by the build. */

struct row_s {
    size_t len;
    seq_t *data;
};

static seq_t *flat          = NULL;   /* flattened sequence array */
static size_t flat_cap      = 0;      /* allocated capacity of flat    */
static size_t flat_len      = 0;      /* number of elements currently stored */

static struct row_s prev1   = {0, NULL};  /* row n-1    */
static struct row_s prev2   = {0, NULL};  /* row n-2    */
static size_t   rows_built = 0;          /* number of rows built so far */

/* Ensure that flat holds at least 'needed' elements. */
static void flat_grow(size_t needed)
{
    if (flat_len + needed > flat_cap) {
        size_t new_cap = flat_cap ? flat_cap * 2 : 64;
        while (new_cap < flat_len + needed)
            new_cap <<= 1;
        flat = (seq_t *)realloc(flat, new_cap * sizeof(seq_t));
        flat_cap = new_cap;
    }
}

/* Build the triangle until the flattened sequence contains at least n elements. */
static void ensure_rows(idx_t n)
{
    /* Convert n to size_t safely (n ≤ 10000 for typical tests). */
    size_t target = (size_t)n;

    while (flat_len < target) {
        if (rows_built == 0) {
            /* Row 0: T(0,0) = 1. */
            size_t len = 1;
            seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
            row[0] = (seq_t)1;
            flat_grow(len);
            flat[flat_len++] = row[0];
            prev2.data = row;
            prev2.len  = len;
            rows_built = 1;
        } else if (rows_built == 1) {
            /* Row 1: T(1,0) = 3. */
            size_t len = 1;
            seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
            row[0] = (seq_t)3;
            flat_grow(len);
            flat[flat_len++] = row[0];
            prev1.data = row;
            prev1.len  = len;
            rows_built = 2;
        } else {
            /* Build next row r = rows_built (starting with r = 2). */
            size_t r    = rows_built;
            size_t len  = (r / 2) + 1;
            seq_t *row  = (seq_t *)malloc(len * sizeof(seq_t));
            for (size_t k = 0; k < len; ++k) {
                seq_t term1 = 0;
                if (k < prev1.len)
                    term1 = prev1.data[k];
                term1 = 2 * term1;                /* 2 * T(n-1,k) */

                seq_t term2 = 0;
                if (k > 0 && (k - 1) < prev2.len)
                    term2 = prev2.data[k - 1];   /* T(n-2,k-1) */
                row[k] = term1 + term2;
            }
            flat_grow(len);
            for (size_t k = 0; k < len; ++k)
                flat[flat_len++] = row[k];

            /* Shift rows: old prev2 is discarded. */
            seq_t *old_prev2 = prev2.data;
            prev2.data = prev1.data;
            prev2.len  = prev1.len;
            prev1.data = row;
            prev1.len  = len;
            free(old_prev2);

            rows_built++;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined for n < 1, return 0 */
    ensure_rows(n);
    return flat[(size_t)(n - 1)];
}
