/*
 * A041669  Denominators of continued fraction convergents to sqrt(353).
 * https://oeis.org/A041669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 29, u64 n <= 59, u128 n <= 115,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 199 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Denominators of continued fraction convergents to sqrt(353)
   Recurrence used (for reference): a(n)=142528*a(n-15)+a(n-30) for n>29
   Computed by the standard recurrence for continued‑fraction convergents. */

#include <stddef.h>   /* for size_t, if needed */

/* Partial quotients of sqrt(353):
   a0 = 18, period of length 15: 1,3,1,2,1,1,1,1,1,1,2,1,3,1,36
   The list contains the values for indices k>=1 (k-1 modulo 15). */
static inline seq_t get_partial(idx_t k)
{
    if (k == 0) return (seq_t)18;                     /* a0 */
    /* static const may be larger than seq_t but values are small */
    static const seq_t period[15] = {
        (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)2,
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)1,
        (seq_t)3, (seq_t)1, (seq_t)36
    };
    return period[(size_t)((k - 1) % 15)];
}

/* Compute a(n) = denominator of the n-th convergent of sqrt(353). */
seq_t a(idx_t n)
{
    seq_t prev2 = (seq_t)1;   /* q_{-2} */
    seq_t prev1 = (seq_t)0;   /* q_{-1} */
    seq_t q = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak = get_partial(k);
        q = ak * prev1 + prev2;
        prev2 = prev1;
        prev1 = q;
    }
    return q;
}
