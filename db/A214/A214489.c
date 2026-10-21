/*
 * A214489  Numbers m such that A070939(m) = A070939(m + A070939(m)), A070939
 *          = length of binary representation.
 * https://oeis.org/A214489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers m such that len(bin(m)) = len(bin(m +
 *            len(bin(m))))
 * Range:     u8 n <= 220, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* index 0 is undefined; return 0 */

    idx_t prev_total = 0;            /* total terms up to previous bit‑length */

    for (idx_t k = 1; ; ++k) {
        idx_t count_k;

        if (k == 1) {
            /* only m = 0 for bit‑length 1 */
            count_k = 1;
        } else {
            /* shift must not exceed the width of idx_t */
            if ((k - 1) >= sizeof(idx_t) * 8) break;
            idx_t pow = ((idx_t)1) << (k - 1);   /* 2^(k-1) */
            if (pow == 0) break;                 /* overflow guard */
            count_k = pow - k;                   /* 2^(k-1) - k */
        }

        idx_t total = prev_total + count_k;      /* count up to current k */

        if (total >= n) {
            idx_t offset = n - prev_total - 1;   /* 0‑based offset inside this block */

            if (k == 1)                      /* only term is 0 */
                return (seq_t)0;

            seq_t base = ((seq_t)((idx_t)1) << (k - 1));   /* 2^(k-1) as seq_t */
            return base + (seq_t)offset;
        }

        prev_total = total;
    }

    /* if n is too large for the available bit‑lengths (rare) */
    return 0;
}
