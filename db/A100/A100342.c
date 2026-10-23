/*
 * A100342  Numerators of the convergents in the continued fraction expansion
 *          for twice the constant given by A100338, where the partial
 *          quotients equal A006519 (greatest power of 2 dividing n)
 *          interleaved with 2's.
 * https://oeis.org/A100342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cofr, nonn
 * Formula:   a(n) = a(1)=2, a(2)=3, * for n≥3: if n odd a(n)=2*a(n-1)+a(n-2);
 *            * if n even a(n)=a(n-1)*v2(n/2)+a(n-2), * where v2(k) is the
 *            largest power of 2 dividing k (A006519)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;

    seq_t prev2 = (seq_t)2; /* a(1) */
    seq_t prev1 = (seq_t)3; /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur;
        if (i & 1) {                 /* odd index */
            cur = 2 * prev1 + prev2;
        } else {                     /* even index */
            uint64_t k     = i / 2;            /* integer division */
            uint64_t pow_v = k & -k;           /* largest power of 2 dividing k */
            cur = prev1 * (seq_t)pow_v + prev2;
        }
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
