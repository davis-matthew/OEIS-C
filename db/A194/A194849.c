/*
 * A194849  Write n = C(i,3)+C(j,2)+C(k,1) with i>j>k>=0; let L[n] = [i,j,k];
 *          sequence gives list of triples L[n], n >= 0.
 * https://oeis.org/A194849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 124, u32 n <= 124, u64 n <= 124,
 *            u128 n <= 124, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A194849.
   For each m = floor(n/3) the triple L[m] = [i,j,k] satisfies
   m = C(i,3)+C(j,2)+k  with i>j>k>=0.
   The function returns the component of L[m] corresponding to
   n%3 (0 → i, 1 → j, 2 → k).  It finds i and j by simple
   incremental searches. */
#include <stdint.h>

static seq_t binom3(idx_t a) {
    seq_t x = a;
    return (x * (x - 1) * (x - 2)) / 6;
}

static seq_t binom2(idx_t a) {
    seq_t x = a;
    return (x * (x - 1)) / 2;
}

seq_t a(idx_t n) {
    /* split the flattened index into triple index and position */
    idx_t m   = n / 3;
    int   pos = n % 3;

    /* find largest i such that C(i,3) <= m */
    idx_t i = 2;
    for (;;) {
        /* C(i+1,3) */
        seq_t next = ((seq_t)(i + 1) * i * (i - 1)) / 6;
        if (next > (seq_t)m) break;
        ++i;
    }
    seq_t C_i3 = binom3(i);
    seq_t r    = (seq_t)m - C_i3;  /* remaining part: C(j,2)+k */

    /* find largest j such that C(j,2) <= r, with j < i */
    idx_t j = 1;
    for (;;) {
        seq_t next = ((seq_t)(j + 1) * j) / 2;  /* C(j+1,2) */
        if (next > r) break;
        ++j;
    }
    seq_t C_j2 = binom2(j);
    seq_t k    = r - C_j2;

    /* return the required component */
    switch (pos) {
        case 0: return (seq_t)i;
        case 1: return (seq_t)j;
        case 2: return (seq_t)k;
    }
    return 0; /* unreachable */
}
