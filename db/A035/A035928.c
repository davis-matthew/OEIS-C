/*
 * A035928  Numbers n such that BCR(n) = n, where BCR =
 *          binary-complement-and-reverse = take one's complement then reverse
 *          bit order.
 * https://oeis.org/A035928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th number n such that BCR(n)=n, where BCR is the
   binary‑complement‑and‑reverse operation.  Numbers are listed in
   increasing order and the OEIS offset is 1. */

#include <stdlib.h>
#include <stdint.h>

/* Helper to push a value into a dynamic array. */
static void push_back(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 64 : (*cap << 1);
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Comparison function for qsort. */
static int seq_cmp(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va == vb) ? 0 : (va < vb ? -1 : 1);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset behaviour is undefined */

    static seq_t *seq = NULL;      /* global sequence array */
    static size_t seq_len = 0;     /* number of elements generated */
    static size_t seq_cap = 0;
    static idx_t next_len = 2;     /* next even length to generate */

    /* Generate terms until we reach the requested index. */
    while (seq_len < (size_t)n) {
        idx_t len = next_len;          /* current even bit length */
        idx_t k = len >> 1;            /* half‑length */

        /* Count of numbers of this length: 2^(k-1). */
        idx_t count = 1ULL << (k - 1);

        /* temporary array for the current length. */
        seq_t *tmp = (seq_t *)malloc((size_t)count * sizeof(seq_t));

        for (idx_t mask = 0; mask < count; ++mask) {
            seq_t val = (seq_t)1 << (len - 1);   /* leading 1 */

            for (idx_t i = 1; i < k; ++i) {
                int bit = (mask >> (i - 1)) & 1ULL;
                if (bit)
                    val |= (seq_t)1 << i;           /* set bit i */
                else
                    val |= (seq_t)1 << (len - 1 - i); /* set complement bit */
            }
            /* trailing bit (position 0) is already 0 */
            tmp[(size_t)mask] = val;
        }

        /* Sort the numbers of this length. */
        qsort(tmp, (size_t)count, sizeof(seq_t), seq_cmp);

        /* Append to the global sequence. */
        for (size_t i = 0; i < (size_t)count; ++i)
            push_back(&seq, &seq_len, &seq_cap, tmp[i]);

        free(tmp);
        next_len += 2;           /* next even length */
    }

    return seq[(size_t)n - 1];
}
