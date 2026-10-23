/*
 * A101883  Write four numbers, skip one, write four, skip two, write four,
 *          skip three... and so on.
 * https://oeis.org/A101883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + 5*⌊(n-1)/4⌋ + ⌊(n-1)/4⌋*(⌊(n-1)/4⌋-1)/2 + ((n-1) mod
 *            4)
 * Range:     u8 n <= 76, u16 n <= 1432, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Assume n >= 1 (index offset 1) */
    idx_t b = (n - 1) / 4;           /* block number */
    idx_t p = (n - 1) % 4;           /* position within the block */

    seq_t term = 1;                  /* base value 1 */
    term += 5 * (seq_t)b;            /* add 5 times the block index */

    if (b > 0) {
        /* add (b * (b-1) / 2) without underflow */
        term += (seq_t)(b * (b - 1) / 2);
    }

    term += (seq_t)p;                /* add position within block */

    return term;
}
