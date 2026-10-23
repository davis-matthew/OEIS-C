/*
 * A153864  Triangle read by rows, A000012 * A153860 * (A066983 * 0^(n-k)).
 * https://oeis.org/A153864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A153864
   Triangle read by rows: T(i,j) = A066983(j) * f(i,j)
   where f(i,j) = 1 if j==i,
                 2 if j>0 and j<i,
                 1 if j==0 and i odd,
                 2 if j==0 and i even.
   The diagonal A066983 satisfies A(0)=A(1)=A(2)=1 and
   A(n)=2*A(n-2)+A(n-3) for n≥3. */
/* External types should be defined by the build: */
#include <stdlib.h>

static seq_t *diag = NULL;          /* diagonal entries A066983 */
static size_t diag_len = 0;         /* number of entries stored so far */
static size_t diag_cap = 0;         /* allocated capacity */

/* Ensure capacity for at least 'need' entries */
static void ensure_diag_cap(size_t need)
{
    if (need <= diag_cap) return;
    size_t new_cap = diag_cap ? diag_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    diag = (seq_t *)realloc(diag, new_cap * sizeof(seq_t));
    diag_cap = new_cap;
}

/* Compute diagonal values up to index 'idx' (inclusive) */
static void compute_diag(idx_t idx)
{
    if (idx < (idx_t)diag_len) return;                /* already have it */
    ensure_diag_cap(idx + 1);
    for (size_t i = diag_len; i <= (size_t)idx; ++i) {
        if (i < 3) {
            diag[i] = (seq_t)1;
        } else {
            diag[i] = (seq_t)2 * diag[i - 2] + diag[i - 3];
        }
    }
    diag_len = (size_t)idx + 1;
}

/* Return the n-th term of A153864 (row–major reading of the triangle) */
seq_t a(idx_t n)
{
    /* Find the row index 'r' such that the first r rows contain <= n terms
       (i.e. (r+1)(r+2)/2 > n). */
    idx_t r = 0;
    for (;;) {
        unsigned __int128 t = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (t <= n)
            ++r;
        else
            break;
    }

    /* Column index within row r */
    unsigned __int128 pre = (unsigned __int128)r * (r + 1) / 2;
    idx_t j = n - (idx_t)pre;                      /* j in [0..r] */

    /* Ensure diagonal up to j */
    compute_diag(j);

    if (j == r) {
        /* On the main diagonal */
        return diag[j];
    } else if (j == 0) {
        /* First column */
        seq_t factor = (r % 2 == 0) ? (seq_t)2 : (seq_t)1;
        return factor * diag[0];
    } else {
        /* Off–diagonal (j>0 and j<r) */
        return (seq_t)2 * diag[j];
    }
}
