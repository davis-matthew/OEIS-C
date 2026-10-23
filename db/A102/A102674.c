/*
 * A102674  Number of digits >= 4 in the decimal representations of all
 *          integers from 0 to n.
 * https://oeis.org/A102674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{k ≥ 0, 10^k ≤ n} ĥk(n) where ĥk(n) counts numbers in
 *            0..n whose digit in the 10^k place is ≥ 4. For each position
 *            with weight w = 10^k: high = ⌊(n / 10) / w⌋ (full 0..9 cycles)
 *            digit = (n / w) % 10 low = n % w ĥk(n) = high * 6 * w + (digit ≥
 *            4 ? ((digit - 4) * w + (low + 1)) : 0)
 * Range:     u8 n <= 226, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    seq_t ans = 0;
    for (idx_t w = 1; w <= n; ) {
        /* high: number of full 10‑digit cycles in this position */
        idx_t high = (n / 10) / w;
        /* digit: the current digit at position w */
        idx_t digit = (n / w) % 10;
        /* low: remaining lower‑position part of n */
        idx_t low = n % w;

        /* full cycles contribution: 6 digits (4…9) per cycle */
        seq_t part_full = (seq_t)high * 6 * (seq_t)w;

        seq_t part_partial = 0;
        if (digit >= 4) {
            part_partial = ((seq_t)(digit - 4)) * (seq_t)w + (seq_t)(low + 1);
        }
        ans += part_full + part_partial;

        /* advance to next position (avoid overflow) */
        if (w > n / 10) break;  /* next w would overflow */
        w *= 10;
    }
    return ans;
}
