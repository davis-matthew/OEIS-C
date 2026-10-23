/*
 * A025809  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^9)).
 * https://oeis.org/A025809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(a,b,c)∈ℕ³ : 2a+5b+9c = n}
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    const idx_t part2 = 2, part5 = 5, part9 = 9;
    idx_t max_c = n / part9;          /* maximum possible number of 9‑parts */

    for (idx_t c = 0; c <= max_c; ++c) {
        idx_t rem = n - part9 * c;    /* remaining value after using c 9’s */
        idx_t b_max = rem / part5;    /* largest possible number of 5‑parts */

        /* b must have the same parity as n - c (because 2a is even) */
        idx_t parity = (n - c) & 1;   /* 0 or 1 */
        idx_t b_start = parity;       /* smallest b ≥ 0 with this parity */

        if (b_max >= b_start) {
            /* count of b values with required parity */
            count += (b_max - b_start) / 2 + 1;
        }
    }
    return count;
}
