/*
 * A037594  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,3.
 * https://oeis.org/A037594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = integer whose base‑8 digits are the first n digits of the
 *            repeating sequence 1,0,3,1,0,3,... Equivalent recurrence:
 *            a(0)=0, a(n)=a(n-1)*8 + d_n where d_n = 1 if n≡1 (mod 3), 0 if
 *            n≡2, 3 if n≡0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;   /* offset is 1, but protect 0 */

    seq_t val = (seq_t)0;
    for (idx_t i = (idx_t)0; i < n; ++i) {
        /* i is 0‑based; position is i+1 */
        idx_t r = (i + (idx_t)1) % (idx_t)3;
        int digit;
        if (r == (idx_t)1) {          /* position ≡1 mod 3 → digit 1 */
            digit = 1;
        } else if (r == (idx_t)2) {   /* position ≡2 mod 3 → digit 0 */
            digit = 0;
        } else {                      /* position ≡0 mod 3 → digit 3 */
            digit = 3;
        }
        val = val * (seq_t)8 + (seq_t)digit;
    }
    return val;
}
