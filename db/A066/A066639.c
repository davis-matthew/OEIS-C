/*
 * A066639  Number of partitions of n with floor(n/2) parts.
 * https://oeis.org/A066639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(ceil(n/2)) for even n where p(k) is the partition
 *            number of k a(n) = p(ceil(n/2)) - 1 for odd n
 * Range:     u8 n <= 32, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Forward declaration of the helper that ensures the
   partition table is filled up to a given index. */
static void ensure_partitions(idx_t max_k);

/* Static storage for partition numbers p(0),p(1),...,p(k). */
static seq_t *partitions = NULL;
static size_t allocated = 0;   /* size of the allocated array */
static size_t list_size  = 0;   /* number of elements that have been computed */

/* Generates partition numbers up to max_k (inclusive) if not already done. */
static void ensure_partitions(idx_t max_k)
{
    if (list_size > (size_t)max_k) return;          /* already computed enough */

    size_t needed = (size_t)max_k + 1;              /* indices 0..max_k are needed */

    /* Increase storage if necessary. */
    if (allocated < needed) {
        size_t new_cap = allocated ? allocated * 2 : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(partitions, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        partitions = tmp;
        allocated = new_cap;
    }

    /* Compute missing partition numbers. */
    if (list_size == 0) {
        partitions[0] = 1;          /* p(0) = 1 */
        list_size = 1;
    }

    for (size_t n = list_size; n <= (size_t)max_k; ++n) {
        seq_t pos = 0, neg = 0;     /* two halves of the pentagonal sum */
        for (int k = 1; ; ++k) {
            int pent1 = k * (3 * k - 1) / 2;
            if (pent1 > (int)n) break;
            if (k & 1) pos += partitions[n - pent1];
            else       neg += partitions[n - pent1];

            int pent2 = k * (3 * k + 1) / 2;
            if (pent2 > (int)n) continue;
            if (k & 1) pos += partitions[n - pent2];
            else       neg += partitions[n - pent2];
        }
        partitions[n] = pos - neg;   /* guaranteed non‑negative */
    }
    list_size = max_k + 1;
}

/* Public function that returns the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; behaviour for n<1 is irrelevant */

    idx_t half = (n + 1) / 2;          /* ceil(n/2) */
    ensure_partitions(half);

    seq_t ans = partitions[half];
    if (n & 1) ans -= 1;                /* subtract 1 for odd n */

    return ans;
}
