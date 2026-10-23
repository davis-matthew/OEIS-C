/*
 * A120165  a(n) = 7 + floor((1 + Sum_{j=1..n-1} a(j))/4).
 * https://oeis.org/A120165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 91, u64 n <= 191,
 *            u128 n <= 389, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS: A120165
 * Recurrence: a(1) = 7
 *              a(n) = 7 + floor((1 + Σ_{j=1}^{n-1} a(j))/4)  for n ≥ 2
 * Derived iterative form that avoids storing large sums:
 * Let r_k = (1 + Σ_{j=1}^{k-1} a(j)) mod 4  (initially r_2 = 1).
 * Then   a(k) = a(k-1) + floor((r_k + a(k-1))/4)
 * and  r_{k+1} = (r_k + a(k-1) mod 4) mod 4
 * This only requires the previous term and a 2‑bit residue, so it works
 * with any fixed‑width seq_t or with arbitrary‑precision.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1, harmless */

    static seq_t *seq = NULL;             /* sequence values, indices 1..cap */
    static size_t cap = 0;                /* largest computed index */
    static unsigned r = 1;                /* (1 + Σ_{j=1}^{k-1} a(j)) mod 4
                                         * for the next term to compute
                                         * (valid for k = cap + 1) */

    /* first call: allocate space for at least n elements */
    if (!seq) {
        size_t new_cap = n > 1 ? n : 1;
        seq = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
        if (!seq) return 0;              /* out of memory – return 0 */
        seq[1] = 7;                      /* a(1) */
        cap = 1;
        r = 1;                            /* r for next term (k=2) */
    }

    /* extend cache if required */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure – return 0 */
        seq = tmp;

        seq_t prev = seq[cap];           /* a(cap) */
        for (size_t k = cap + 1; k <= new_cap; ++k) {
            seq_t diff = (r + prev) / 4;          /* floor((r+prev)/4) */
            seq_t next = prev + diff;             /* a(k) */
            seq[k] = next;
            r = (r + (unsigned)(prev % 4)) & 3;   /* update residue for next term */
            prev = next;
        }
        cap = new_cap;
    }

    return seq[(size_t)n];
}
