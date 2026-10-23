/*
 * A138685  Numbers k such that there is no prime of the form 2k + p^2 for any
 *          prime p.
 * https://oeis.org/A138685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 7647, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A138685 – Numbers k such that k ≡ 1 (mod 3) and 2k+9 is composite.
   A(n) is the n‑th integer of the form 3m+1 with 6m+11 composite. */
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* n starts at 1 (offset 1) */
    idx_t count = 0;
    idx_t m = 0;
    while (1) {
        seq_t val = (seq_t)6 * m + 11;   /* 2k+9 = 6m+11 */
        if (!is_prime_small(val)) {
            ++count;
            if (count == n)
                return (seq_t)3 * m + 1;  /* k = 3m+1 */
        }
        ++m;
    }
}
