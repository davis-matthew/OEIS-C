/*
 * A107364  Numbers of the form (3^i)*(13^j).
 * https://oeis.org/A107364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sorted{3^i * 13^j | i,j >= 0} (Hamming numbers with
 *            primes 3 and 13)
 * Range:     u8 n <= 9, u16 n <= 30, u32 n <= 102, u64 n <= 381,
 *            u128 n <= 1458, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t size   = 0;
    static idx_t cap    = 0;
    static idx_t i3 = 0, i13 = 0;

    if (n == 0)
        return 0; /* undefined for n < 1, but return 0 for safety */

    /* Initialize on first call */
    if (seq == NULL) {
        cap   = 2;
        seq   = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) abort();
        seq[0] = (seq_t)1;      /* a(1) = 1 */
        size = 1;
        i3 = i13 = 0;
    }

    /* Resize if necessary */
    if (cap < n) {
        cap = cap ? cap << 1 : 1;
        while (cap < n) cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
    }

    /* Generate new terms up to n */
    while (size < n) {
        seq_t cand3  = seq[i3] * (seq_t)3;
        seq_t cand13 = seq[i13] * (seq_t)13;
        seq_t next = (cand3 < cand13) ? cand3 : cand13;
        seq[size++] = next;
        if (next == cand3) i3++;
        if (next == cand13) i13++;
    }

    return seq[n - 1];
}
