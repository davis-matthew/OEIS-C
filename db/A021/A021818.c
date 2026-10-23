/*
 * A021818  Decimal expansion of 1/814.
 * https://oeis.org/A021818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pattern of the decimal expansion of 1/814:
   a(0) = 0, a(1) = 0, and for n >= 2:
   a(n) = [1, 2, 2, 8, 5, 0][(n-2) % 6] */
seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;
    }
    /* Compute the position within the repeating 6‑digit block. */
    idx_t pos = (n - 2) % 6;
    switch (pos) {
        case 0: return 1;
        case 1: return 2;
        case 2: return 2;
        case 3: return 8;
        case 4: return 5;
        case 5: return 0;
    }
    /* Unreachable, but keep the compiler happy. */
    return 0;
}
