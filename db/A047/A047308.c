/*
 * A047308  Numbers that are congruent to {0, 3, 4, 5, 6} mod 7.
 * https://oeis.org/A047308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*floor((n-1)/5) + w[(n-1)%5] where w = {0,3,4,5,6}
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers congruent to {0,3,4,5,6} modulo 7 (offset 1) */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* behaviour below the offset is irrelevant */

    idx_t q = (n - 1) / 5;             /* number of complete 5‑valid‑number blocks */
    idx_t r = (n - 1) % 5;             /* position within the current block (0‑based) */

    /* offsets of the 5 valid remainders in each block of 7 consecutive integers */
    static const uint8_t offsets[5] = {0, 3, 4, 5, 6};

    return (seq_t)q * 7 + offsets[r];
}
