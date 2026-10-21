/*
 * A057016  Consider recurrence b(0) = (2n+1)/2, b(n) = b(n-1)*floor(b(n-1));
 *          sequence gives first integer reached.
 * https://oeis.org/A057016
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: b(0) = (2n+1)/2; b(k) = b(k-1)*floor(b(k-1));
   a(n) is the first integer reached, namely b(k)/2 when b(k) becomes even. */

seq_t a(idx_t n)
{
    /* for n < 2 the sequence is undefined, but we return 0 for safety */
    if (n < 2)
        return 0;

    seq_t val = (seq_t)n * (seq_t)2 + 1;   /* val = 2n+1  (numerator of b(0)) */

    /* keep multiplying by floor(b) until b becomes an integer (i.e., val even) */
    while ((val & 1) != 0)                /* while b is not integer */
    {
        seq_t floor_val = val >> 1;       /* floor(val/2) */
        val *= floor_val;                /* b = b * floor(b) */
    }

    return val >> 1;   /* result is b/2 when b is integer */
}
