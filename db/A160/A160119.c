/*
 * A160119  A three-dimensional version of the cellular automaton A160118,
 *          using cubes.
 * https://oeis.org/A160119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=27. For n≥3: If n even:
 *            a(n)=27+208*Σ_{k=1}^{n/2-1} 7^{wt(k)-1} If n odd :
 *            a(n)=27+8*Σ_{k=1}^{(n+1)/2-1} 7^{wt(k)-1}
 *            +200*Σ_{k=1}^{(n+1)/2-2} 7^{wt(k)-1} where wt(k) is the number
 *            of 1‑bits in the binary representation of k
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned int popcount64(idx_t x) {
    unsigned int c = 0;
    while (x) {
        x &= x - 1U;
        ++c;
    }
    return c;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)27;

    /* Determine the maximum k needed for cumulative sums */
    idx_t max_k;
    if (n & 1)          /* odd n */
        max_k = (n + 1) / 2;   /* (n+1)/2 - 1 is used for sums */
    else
        max_k = n / 2;         /* (n/2) - 1 is used for sums */

    if (max_k < 1) max_k = 1;     /* safety */

    /* Allocate cumulative array B[k] = Σ_{i=1}^{k} 7^{wt(i)-1} */
    seq_t *B = (seq_t *)malloc((max_k + 1) * sizeof(seq_t));
    if (!B) return (seq_t)0;     /* malloc failure – unlikely */

    B[0] = (seq_t)0;
    for (idx_t k = 1; k <= max_k; ++k) {
        unsigned int pc = popcount64(k);
        seq_t term = (seq_t)1;
        for (unsigned int i = 1; i < pc; ++i)
            term *= (seq_t)7;   /* 7^(pc-1) */
        B[k] = B[k - 1] + term;
    }

    seq_t result = (seq_t)27;
    if (n & 1) {   /* odd */
        idx_t half = (n + 1) / 2;
        seq_t s1 = B[half - 1];
        seq_t s2 = (half >= 2) ? B[half - 2] : (seq_t)0;
        result += (seq_t)8 * s1 + (seq_t)200 * s2;
    } else {      /* even */
        idx_t half = n / 2;
        seq_t s = B[half - 1];
        result += (seq_t)208 * s;   /* 8 + 200 = 208 */
    }

    free(B);
    return result;
}
