/*
 * A060445  "Dropping time" in 3x+1 problem starting at 2n+1 (number of steps
 *          to reach a lower number than starting value). Also called
 *          glide(2n+1).
 * https://oeis.org/A060445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 4726, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the dropping time of the Collatz trajectory starting at 2*n+1.
   The dropping time is the number of steps needed to reach a value
   lower than the starting value.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 by definition */

    seq_t start = ((seq_t)n << 1) | 1;   /* start = 2*n + 1, odd */
    seq_t cur   = start;
    idx_t  steps = 0;

    while (cur >= start) {
        if ((cur & 1) == 0)
            cur >>= 1;            /* even case: divide by 2 */
        else
            cur = cur * 3 + 1;    /* odd case: 3x + 1 */
        ++steps;
    }

    return (seq_t)steps;
}
