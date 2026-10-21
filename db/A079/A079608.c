/*
 * A079608  Marks on lexicographically earliest 21-mark optimal Golomb ruler.
 * https://oeis.org/A079608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: marks of the lexicographically earliest 21‑mark optimal Golomb ruler */
seq_t a(idx_t n)
{
    /* Mark positions for the 21‑mark optimal Golomb ruler (minimal length 333) */
    static const seq_t marks[21] = {
        0, 2, 24, 56, 77, 82, 83, 95, 129, 144,
        179, 186, 195, 255, 265, 285, 293, 296, 310, 329, 333
    };
    if (n < 1 || n > 21U) return 0;
    return marks[n - 1];
}
