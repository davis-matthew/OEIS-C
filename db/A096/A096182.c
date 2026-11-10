/*
 * A096182  Index of first occurrence of n in A095773.
 * https://oeis.org/A096182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that b(k) = n, where b = A095773: b(1) = 1,
 *            b(k) = 1 + b(k - b(b(b(k-1)))); computed by generating b until
 *            it reaches n
 * Range:     u8 n <= 48, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static idx_t *b = NULL;      /* b[i] = A095773(i) */
    static idx_t *first = NULL;  /* first[v] = first index with b = v */
    static size_t cap = 0;
    static idx_t cnt = 0;        /* number of b terms computed */
    static idx_t maxv = 0;       /* largest value seen in b */

    if (n == 0) return (seq_t)0;

    while (maxv < n) {
        idx_t i = cnt + 1;
        if ((size_t)i >= cap) {
            size_t nc = cap ? cap * 2 : 64;
            idx_t *nb = (idx_t *)realloc(b, nc * sizeof(idx_t));
            if (!nb) exit(1);
            b = nb;
            idx_t *nf = (idx_t *)realloc(first, nc * sizeof(idx_t));
            if (!nf) exit(1);
            first = nf;
            cap = nc;
        }
        idx_t v;
        if (i == 1) {
            v = 1;
        } else {
            idx_t t = b[b[b[i - 1]]];
            v = 1 + b[i - t];
        }
        b[i] = v;
        cnt = i;
        if (v > maxv) {
            maxv = v;
            first[v] = i;
        }
    }
    return (seq_t)first[n];
}
