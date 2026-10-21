/*
 * A219388  Basic quantic arrangement for the 1 to 120 planetary electrons and
 *          elementary periods (circles I to XX) distributed by energy levels.
 * https://oeis.org/A219388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, less
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A219388: repeating pattern 8,7,7,6,6,6,5,5,5,5,4,4,4,4,3,3,3,2,2,1
   The sequence repeats every 20 terms, so a(n) is the (n‑1) mod 20‑th entry
   of this fixed array. */
seq_t a(idx_t n)
{
    /* 20‑term repeating block */
    static const seq_t pattern[20] = {
        8, 7, 7, 6, 6, 6, 5, 5, 5, 5,
        4, 4, 4, 4, 3, 3, 3, 2, 2, 1
    };

    /* n is guaranteed to be ≥ 1; modulo operation works with unsigned type */
    idx_t idx = (n - 1) % 20u;
    return pattern[idx];
}
