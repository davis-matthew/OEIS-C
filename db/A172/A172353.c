/*
 * A172353  Triangle t(n,k) of Padovan factorial ratios c(n)/(c(k)*c(n-k))
 *          where c(n) = A126772(n).
 * https://oeis.org/A172353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = t(r,k) where r is the row satisfying * r*(r+1)/2 <= n <
 *            (r+1)*(r+2)/2, * k = n - r*(r+1)/2. * t(r,k) = c(r) / (c(k) *
 *            c(r-k)), * c(n) = product of the first n Padovan numbers, *
 *            Padovan: P(0)=P(1)=P(2)=1, P(n)=P(n-2)+P(n-3) for n>=3
 * Range:     u8 n <= 45, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static storage for Padovan numbers and their cumulative products. */
static seq_t *padovan      = NULL; /* padovan[i] = P(i) for i = 0 .. padovan_size-1   */
static seq_t *cp           = NULL; /* cp[i]   = product_{j=0}^{i-1} P(j), cp[0] = 1 */
static idx_t  padovan_size = 0;    /* number of Padovan entries stored */

/* Ensure that Padovan numbers up to index max_needed and the
 * corresponding cumulative products are computed. */
static void
ensure_precomp(idx_t max_needed)
{
    if (max_needed < padovan_size)
        return;

    idx_t new_size = max_needed + 1;            /* need indices 0 .. max_needed */
    seq_t *new_pad = (seq_t *)malloc(sizeof(seq_t) * new_size);
    seq_t *new_cp  = (seq_t *)malloc(sizeof(seq_t) * (new_size + 1));
    if (!new_pad || !new_cp) {
        /* Out of memory: abort the program because the sequence cannot
           be produced without the required data. */
        abort();
    }

    /* Compute Padovan sequence. */
    if (new_size > 0) new_pad[0] = (seq_t)1;
    if (new_size > 1) new_pad[1] = (seq_t)1;
    if (new_size > 2) new_pad[2] = (seq_t)1;
    for (idx_t i = 3; i < new_size; ++i)
        new_pad[i] = new_pad[i - 2] + new_pad[i - 3];

    /* Compute cumulative products: cp[i] = product_{j=0}^{i-1} P(j). */
    new_cp[0] = (seq_t)1;
    for (idx_t i = 1; i <= new_size; ++i)
        new_cp[i] = new_cp[i - 1] * new_pad[i - 1];

    /* Replace old buffers. */
    if (padovan) free(padovan);
    if (cp)      free(cp);

    padovan      = new_pad;
    cp           = new_cp;
    padovan_size = new_size;
}

/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.  */
static idx_t
find_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* Grow high until the start of row (high+1) exceeds n. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }

    idx_t r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 start = (unsigned __int128)mid * (mid + 1) / 2;
        if (start <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return r;
}

/* Main entry: a(n) = t(row, col) flattened row‑major. */
seq_t
a(idx_t n)
{
    /* Determine row r and column k. */
    idx_t r = find_row(n);
    unsigned __int128 start_r = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)start_r;

    /* Ensure Padovan numbers and cumulative products are available. */
    ensure_precomp(r);

    /* Compute t(r,k) = cp[r] / (cp[k] * cp[r-k]). */
    seq_t result = cp[r] / cp[k];
    result /= cp[r - k];
    return result;
}
