/*
 * A067979  Triangle read by rows of incomplete convolutions of Lucas numbers
 *          L(n+1) = A000204(n+1), n>=0.
 * https://oeis.org/A067979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = Σ_{k=0..m} L(k+1) * L(r-k+1) * where (r,m) are the row
 *            and column of the flattened index n, * r = ⌊(√(8n+1)-1)/2⌋ , * m
 *            = n - r(r+1)/2 , * and L(i) are Lucas numbers, L(0)=2, L(1)=1,
 *            L(i)=L(i-1)+L(i-2)
 * Range:     u8 n <= 25, u16 n <= 146, u32 n <= 732, u64 n <= 3453,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/int_sqrt_u128.h"

/* Cached Lucas numbers (zero‑based index).  Index i stores L(i). */
static seq_t *lucas = NULL;
static size_t lucas_len = 0;

/* Ensure that lucas array has entries 0..max_index inclusive. */
static void ensure_lucas(size_t max_index)
{
    if (max_index < lucas_len)
        return;
    size_t new_len = max_index + 1;
    seq_t *new_lucas = (seq_t *)realloc(lucas, new_len * sizeof(seq_t));
    if (!new_lucas)
        abort();
    lucas = new_lucas;
    if (lucas_len == 0) {
        /* Base values: L(0)=2, L(1)=1. */
        lucas[0] = (seq_t)2;
        if (new_len > 1)
            lucas[1] = (seq_t)1;
        lucas_len = 2;
    }
    for (size_t i = lucas_len; i < new_len; ++i) {
        lucas[i] = lucas[i-1] + lucas[i-2];
    }
    lucas_len = new_len;
}

seq_t a(idx_t n)
{
    /* Determine row r and column m from flattened index n. */
    unsigned __int128 eight_n_plus_1 = (unsigned __int128)8 * n + 1;
    unsigned long long t = int_sqrt_u128(eight_n_plus_1);
    idx_t r = (idx_t)((t - 1) / 2);
    unsigned __int128 start = (unsigned __int128)r * (r + 1) / 2;
    idx_t m = (idx_t)(n - (uint64_t)start);   /* column in the r‑th row */

    /* Ensure Lucas numbers up to index r+1 are available. */
    ensure_lucas((size_t)r + 1);

    /* Compute the convolution sum. */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= m; ++k) {
        sum += lucas[(size_t)k + 1] * lucas[(size_t)r - k + 1];
    }
    return sum;
}
