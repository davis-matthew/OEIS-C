/*
 * A152736  Triangle read by rows: M*Q, where M = an infinite lower triangular
 *          matrix with A140456 in every column: (1, 1, 1, 3, 7, 23, 71, ...)
 *          and Q = a matrix with A000085 as the main diagonal the rest zeros.
 * https://oeis.org/A152736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  eigen, nonn, tabl
 * Formula:   a(n) is the flattened entries of the triangle with * a(i,j) =
 *            A140456(i-j+1) * A000085(j-1) * where A000085 is the involution
 *            sequence (offset 0) and * A140456 satisfies * sum_{k=1..m}
 *            A140456(k)*A000085(m-k) = A000085(m) * (convolution inverse of
 *            A000085). * * The function computes general terms...
 * Range:     u8 n <= 36, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "../utils/grow_seq_array.h"   /* allocate and grow helpers */

/* static caches for the two base sequences */
static seq_t *a140 = NULL;          /* A140456, 1‑based index */
static idx_t  a140_len = 0;         /* last filled index of a140      */
static size_t a140_cap = 0;         /* allocated capacity of a140      */

static seq_t *a000 = NULL;          /* A000085, 0‑based index */
static idx_t  a000_len = 0;         /* last filled index of a000      */
static size_t a000_cap = 0;         /* allocated capacity of a000      */

/* ensure that A000 has entries 0..n (inclusive) */
static void ensure_A000(idx_t n)
{
    if (a000_len > n) return;              /* already have enough          */
    size_t new_cap = a000_cap ? a000_cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1U;

    /* grow or allocate the array */
    grow_seq_array(&a000, &a000_cap, new_cap);
    /* initialise new entries */
    for (idx_t i = a000_len; i <= n; ++i) {
        if (i == 0)                a000[0] = (seq_t)1;
        else if (i == 1)           a000[1] = (seq_t)1;
        else
            a000[i] = a000[i-1] + (seq_t)(i-1) * a000[i-2];
    }
    a000_len = n + 1;   /* indices 0..n inclusive are ready */
}

/* ensure that A140 has entries 1..k (inclusive) */
static void ensure_A140(idx_t k)
{
    if (a140_len >= k) return;           /* already have enough           */
    /* first make sure the underlying A000 sequence is ready up to k */
    ensure_A000(k);

    size_t new_cap = a140_cap ? a140_cap : 16;
    while (new_cap <= (size_t)k) new_cap <<= 1U;
    grow_seq_array(&a140, &a140_cap, new_cap);

    /* compute missing terms of A140 */
    for (idx_t i = a140_len + 1; i <= k; ++i) {
        seq_t sum = 0;
        for (idx_t j = 1; j < i; ++j)
            sum += a140[j] * a000[i-j];
        a140[i] = a000[i] - sum;
    }
    a140_len = k;
}

/* helper: triangular number t(r) = r*(r+1)/2 (using 128‑bit to avoid overflow) */
static inline idx_t triangular(idx_t r)
{
    return (idx_t)((__int128)r * (r + 1) / 2);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0             */

    /* find row i such that triangular(i-1) < n ≤ triangular(i) */
    idx_t i = (idx_t)((sqrtl((long double)8 * n + 1) - 1) / 2);
    while (triangular(i) < n) ++i;
    while (i > 0 && triangular(i-1) >= n) --i;

    idx_t j = n - triangular(i-1);          /* column index (1‑based)    */
    idx_t k = i - j + 1;                    /* A140 index               */

    /* ensure the underlying sequences are long enough */
    ensure_A140(k);
    ensure_A000(i-1);                       /* need up to i‑1 for A000[j‑1] */

    return a140[k] * a000[j-1];
}
