/*
 * A037575  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 2,1,2.
 * https://oeis.org/A037575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((212/999) * 10^n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Equivalently, a(n) is the integer whose decimal representation
    consists of the first n terms of the repeating block 2,1,2.
    A direct construction uses the observation that for every
    complete block of 3 digits we multiply the current value by 1000
    and add 212.  The remaining 0‑, 1‑, or 2‑digit prefix is handled
    after the block loop. */
seq_t a(idx_t n)
{
    /* Handle the b‑file offset of the sequence (n ≥ 1). */
    if (n == 0)
        return (seq_t)0;

    idx_t blocks = n / 3;          /* number of complete 212–blocks   */
    seq_t   val = 0;               /* will hold the integer value      */

    if (blocks > 0) {
        /* Initialise with the first block. */
        val = (seq_t)212;
        /* Append remaining complete blocks. */
        for (idx_t i = 1; i < blocks; ++i) {
            val = val * (seq_t)1000 + (seq_t)212;
        }
    }

    /* Handle the remaining 0, 1 or 2 digits after the last full block. */
    switch (n % 3) {
        case 0:            /* no additional digits            */
            return val;
        case 1:            /* append '2'                      */
            return val * (seq_t)10 + (seq_t)2;
        case 2:            /* append '21'                     */
            return val * (seq_t)100 + (seq_t)21;
    }
    /* unreachable */
    return val;
}
