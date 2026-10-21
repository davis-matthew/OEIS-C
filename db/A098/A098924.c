/*
 * A098924  45-gonal numbers: n*(43*n-41)/2.
 * https://oeis.org/A098924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(43*n-41)/2
 * Range:     u8 n <= 3, u16 n <= 55, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is undefined for n = 0; return 0 if requested. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Cast the index to the sequence type and use 128‑bit arithmetic
       to avoid intermediate overflow for all widths of seq_t that
       still fit the final value. */
    seq_t tn   = (seq_t)n;
    unsigned __int128 part1 = (unsigned __int128)tn * 43 - 41;
    unsigned __int128 prod   = (unsigned __int128)tn * part1;
    seq_t result = (seq_t)(prod / 2);
    return result;
}
