/*
 * A047479  Numbers that are congruent to {0, 1, 5, 7} mod 8.
 * https://oeis.org/A047479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + offset, with offset = {0,1,5,7}[ (n-1)
 *            mod 4 ]
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047479: numbers congruent to {0,1,5,7} mod 8, offset 1 */

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1; for n == 0 the result is unused */
    idx_t t = n - 1;          /* shift to 0‑based index */
    idx_t k = t / 4;          /* block number */
    idx_t r = t % 4;          /* position inside block */

    seq_t value = (seq_t)8 * (seq_t)k;   /* base value 8k */

    /* add the appropriate offset for the block position */
    switch (r) {
        case 0: /* 8k + 0 */
            break;
        case 1: /* 8k + 1 */
            value += (seq_t)1;
            break;
        case 2: /* 8k + 5 */
            value += (seq_t)5;
            break;
        case 3: /* 8k + 7 */
            value += (seq_t)7;
            break;
    }

    return value;
}
