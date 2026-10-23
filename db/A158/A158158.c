/*
 * A158158  Number of nondecreasing integer sequences of length 24 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{q=0}^{24} Σ_{p=0}^{24−q} P(n,q)·P(n,p) where P(n,k) is
 *            the number of partitions of n into exactly k parts
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 45, u64 n <= 178, u128 n <= 268,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAXK 24                           /* maximum number of parts */

static seq_t *partitions = NULL;          /* flat array of size (max_n+1)*(MAXK+1) */
static idx_t  max_cached  = 0;            /* largest n for which table is valid */

#define PART(n,k) (partitions[((n)*(MAXK+1)) + (k)])

static void ensure_partitions(idx_t n)
{
    if (n <= max_cached)
        return;

    idx_t oldmax = max_cached;
    idx_t newmax = n;

    size_t newbytes = (size_t)(newmax + 1) * (MAXK + 1) * sizeof(seq_t);
    seq_t *new_ptr = (seq_t *)realloc(partitions, newbytes);
    if (!new_ptr)
        abort();                          /* out of memory */
    partitions = new_ptr;

    /* Initialise base row for n = 0 if it hasn't been done yet */
    if (oldmax == 0 && newmax >= 0) {
        for (idx_t k = 0; k <= MAXK; ++k)
            PART(0, k) = (k == 0 ? (seq_t)1 : (seq_t)0);
    }

    /* Compute new rows using recurrence for exact partitions */
    for (idx_t i = oldmax + 1; i <= newmax; ++i) {
        PART(i, 0) = 0;                   /* no partitions of positive n into 0 parts */
        for (idx_t k = 1; k <= MAXK; ++k) {
            if (i < k) {
                PART(i, k) = 0;
            } else {
                PART(i, k) = PART(i - 1, k - 1) + PART(i - k, k);
            }
        }
    }

    max_cached = newmax;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* undefined at position 0 */

    ensure_partitions(n);

    seq_t sum = 0;
    for (idx_t q = 0; q <= MAXK; ++q) {
        for (idx_t p = 0; p <= MAXK - q; ++p) {
            sum += PART(n, q) * PART(n, p);
        }
    }
    return sum;
}
