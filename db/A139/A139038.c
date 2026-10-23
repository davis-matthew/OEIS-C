/*
 * A139038  Triangle read by rows: T(n,m) = A000931(m+6) if m <= floor(n/2),
 *          A000931(n+6-m) otherwise, for 0 <= m <= n.
 * https://oeis.org/A139038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) is read by rows from the triangle T(n,m) defined by T(n,m)
 *            = Padovan(m+6) if m <= floor(n/2) T(n,m) = Padovan(n+6-m)
 *            otherwise The Padovan sequence here has initial terms
 *            Padovan(0)..Padovan(7) = 1 and for k >= 8 Padovan(k) =
 *            Padovan(k-2) + Padovan(k-3)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

static seq_t *pad_seq = NULL;   /* storage for Padovan values */
static idx_t pad_len = 0;       /* number of pre‑computed values */

/* Ensure that Padovan values are computed up to required index 'needed' */
static void ensure_pad(idx_t needed)
{
    if (pad_len > needed) return;

    idx_t new_len = needed + 1;
    seq_t *new_ptr;

    if (pad_seq == NULL)
    {
        new_ptr = (seq_t *)malloc(new_len * sizeof(seq_t));
    }
    else
    {
        new_ptr = (seq_t *)realloc(pad_seq, new_len * sizeof(seq_t));
    }

    if (new_ptr == NULL) exit(1);          /* allocation failed */
    pad_seq = new_ptr;

    /* initialise the first 8 terms to 1 */
    for (idx_t i = 0; i < new_len && i < 8; ++i)
        pad_seq[i] = (seq_t)1;

    /* compute the remaining terms */
    idx_t start = (pad_len > 8) ? pad_len : 8;
    for (idx_t i = start; i < new_len; ++i)
        pad_seq[i] = pad_seq[i - 2] + pad_seq[i - 3];

    pad_len = new_len;
}

/* Return the n-th term of A139038 */
seq_t a(idx_t n)
{
    /* Find the row r and column m such that
       r(r+1)/2 <= n < (r+1)(r+2)/2   and   m = n - r(r+1)/2   */
    idx_t r;
    long double disc = sqrtl((long double)1 + 8.0L * n);
    r = (idx_t)((-1.0L + disc) / 2.0L);

    /* Correct possible rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t m = n - r * (r + 1) / 2;
    idx_t half = r >> 1;                /* floor(r/2) */

    idx_t index = (m <= half) ? (m + 6) : (r + 6 - m);

    ensure_pad(index);
    return pad_seq[index];
}
