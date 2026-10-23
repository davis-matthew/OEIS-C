/*
 * A118297  Start with 22 and repeatedly reverse the digits and add 1 to get
 *          the next term.
 * https://oeis.org/A118297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=22; for n>1, a(n)=reverse(a(n-1))+1. After reaching the
 *            value 100 (n=16), the sequence cycles through 2,3,4,5,6,7,8,9,10
 *            with period 9. Thus for n≥17: a(n)=2+((n-17) mod 9)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse.h"

seq_t a(idx_t n)
{
    /* Special case: compute directly for the first 16 terms. */
    if (n <= (idx_t)16) {
        seq_t cur = (seq_t)22;
        for (idx_t i = (idx_t)1; i < n; ++i) {
            cur = reverse(cur) + (seq_t)1;
        }
        return cur;
    }

    /* For n≥17 the sequence repeats 9 values: 2..10. */
    idx_t offset = (n - (idx_t)17) % (idx_t)9;   /* 0…8 */
    return (seq_t)2 + (seq_t)offset;
}
