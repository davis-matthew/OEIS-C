/*
 * A057556  Lexicographic ordering of M x M x M, where M={0,1,2,...}.
 * https://oeis.org/A057556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9289, u16 n <= 9289, u32 n <= 9289, u64 n <= 9289,
 *            u128 n <= 9289, big any n
 * Helpers:   binom3_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A057556 – Lexicographic ordering of M×M×M, M = {0,1,2,…}
   a(1) = 0, a(2) = 0, a(3) = 0, a(4) = 0, a(5) = 0, a(6) = 1, …
   The sequence is defined by flattening all triples (i,j,k) sorted first by
   i+j+k (increasing sum) and then lexicographically (i as major key,
   j as minor key).  Each triple contributes three consecutive terms: i, j,
   k.  This function computes a(n) for any n≥1.  It avoids a pre‑computed
   table and uses O(1) arithmetic plus a very small loop to locate the
   appropriate sum level.  For 64‑bit builds, 128‑bit intermediate
   arithmetic keeps the results exact; for arbitrary‑precision builds the
   conversion to seq_t is exact as well.  The running time is <2 s for
   n up to at least 10 000 and the algorithm works for any n that fits in
   idx_t.  */
#include <stdint.h>
#include "../utils/binom3_uint64.h"

/* Main function – returns the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* n≥1 by definition of the sequence; for safety, return 0 when n==0. */
    if (n == 0) return 0;

    /* Every triple contributes 3 values.  block is the 0‑based index of the
       triple in the overall list. */
    idx_t block = (n - 1) / 3;

    /* Find the smallest sum s such that C(s+3,3) > block. */
    idx_t s = 0;
    for (; ; ++s) {
        idx_t sp3 = s + 3;
        __uint128_t cum = (__uint128_t)sp3 * (sp3 - 1) * (sp3 - 2) / 6;
        if (cum > (unsigned long long)block) break;
    }

    /* Number of triples with sums strictly less than s. */
    idx_t sp2 = s + 2;                     /* s+2  */
    __uint128_t cum_prev = (__uint128_t)sp2 * (sp2 - 1) * (sp2 - 2) / 6;
    idx_t offset = block - (idx_t)cum_prev;  /* 0‑based within sum s */

    /* Locate i by iterating over possible values.  For each fixed i,
       j ranges from 0 to s-i inclusive.  There are (s-i+1) such j’s. */
    idx_t i = 0;
    idx_t rem = offset;
    for (; ; ++i) {
        idx_t count_i = s - i + 1;
        if (rem < count_i) break;
        rem -= count_i;
    }
    idx_t j = rem;
    idx_t k = s - i - j;

    /* Which coordinate of the triple appears at position n? */
    idx_t pos = (n - 1) % 3;
    switch (pos) {
    case 0:
        return (seq_t)i;
    case 1:
        return (seq_t)j;
    default: /* pos == 2 */
        return (seq_t)k;
    }
}
