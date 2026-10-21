/*
 * A088697  Replace 0 with 10 in binary representation of n.
 * https://oeis.org/A088697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) replaces every 0 in the binary representation of n by 10.
 *            Recurrence: a(0)=0, a(2n)=4·a(n)+2, a(2n+1)=2·a(n)+1
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by traversing the bits of n from most‑significant to
   least‑significant and building the resulting number bit by bit. */
seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0)
        return 0;

    seq_t result = 0;

    /* Find position of the most significant set bit of n. */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp >> 1) {
        ++msb;
        tmp >>= 1;
    }

    /* Process bits from msb down to 0. */
    for (idx_t i = msb; ; --i) {
        if ((n >> i) & 1)     /* bit is 1: append '1' */
            result = (result << 1) | 1;
        else                  /* bit is 0: append '10' */
            result = (result << 2) | 2;

        if (i == 0)
            break;           /* last bit processed */
    }

    return result;
}
