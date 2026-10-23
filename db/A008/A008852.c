/*
 * A008852  Numbers n such that n^2 and n have same last 2 digits.
 * https://oeis.org/A008852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(4n) = 100*n; a(4n+1) = 100*n+1; a(4n+2) = 100*n+25; a(4n+3) =
 *            100*n+76
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)       /* offset is 1; return 0 for out-of-range index */
        return 0;

    idx_t k = (n - 1) / 4;   /* block index */
    idx_t r = (n - 1) % 4;   /* position within block */

    seq_t result = (seq_t)k * 100;   /* 100 * block index */

    switch (r) {
        case 0: break;
        case 1: result += 1;  break;
        case 2: result += 25; break;
        case 3: result += 76; break;
    }
    return result;
}
