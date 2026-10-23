/*
 * A108336  Unique sequence of 1's and 0's such that (Sum_{n >= 0} a(n)*x^n)^2
 *          mod 4 has coefficients which are all 1's and 2's (A083952).
 * https://oeis.org/A108336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0)=1; for n>0 choose a(n)∈{0,1} so that ∑_{i=0}^n a(i)·a(n‑i)
 *            ≡ 1 or 2 (mod 4). The sequence is uniquely determined by this
 *            property
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t size   = 0;          /* number of terms already computed */
    static idx_t capacity = 0;        /* allocated capacity */

    if (capacity < n + 1) {
        idx_t newcap = capacity ? capacity << 1U : 1;
        while (newcap < n + 1) newcap <<= 1U;
        seq = seq_realloc(seq, newcap);
        capacity = newcap;
    }

    for (idx_t k = size; k <= n; ++k) {
        if (k == 0) {                 /* a(0)=1 */
            seq[0] = 1;
            continue;
        }

        seq_t S0 = 0;                  /* sum modulo 4 of pairs i<k-i */
        idx_t i = 1, j = k - 1;
        while (i <= j) {
            if (i == j) {              /* i==j => a(i)*a(k-i) counted once */
                S0 = (S0 + seq[i]) & 3;
            } else {                   /* two opposite terms contribute 2*a*i* a_j */
                seq_t prod = seq[i] & seq[j];
                S0 = (S0 + (prod << 1)) & 3;
            }
            ++i;
            --j;
        }

        seq_t c0 = S0;                 /* coefficient if a(k)=0 */
        seq_t c1 = (S0 + 2) & 3;       /* a(k)=1 adds 2 from a0·a(k)+a(k)·a0 */
        if ((k & 1) == 0) {            /* even k: add self‑pair a(k/2)² */
            seq_t mid = seq[k >> 1];
            c1 = (c1 + mid) & 3;
        }

        /* the sequence is uniquely defined: exactly one of c0, c1 is
           congruent to 1 or 2 modulo 4. */
        seq[k] = (c0 == 1 || c0 == 2) ? 0 : 1;
    }

    size = n + 1;
    return seq[n];
}
