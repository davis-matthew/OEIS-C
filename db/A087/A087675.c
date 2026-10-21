/*
 * A087675  Consider recurrence b(0) = (2n+1)/2, b(n) = b(0)*floor(b(n-1));
 *          sequence gives first integer reached.
 * https://oeis.org/A087675
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = first integer reached by recurrence * b(0) = (2n+1)/2 , *
 *            b(k) = b(0) * floor(b(k-1)) (k ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 32, u64 n <= 256, u128 n <= 2048,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* b(0) numerator */
    seq_t t = (seq_t)n * 2 + 1;      /* (2n+1) */
    seq_t num = t;                   /* current numerator, denominator = 2 */

    /* iterate until numerator becomes even (i.e. the value is integer) */
    while (num & 1)
    {
        seq_t floor_cur = num >> 1;  /* floor(num/2) */
        num = t * floor_cur;         /* next numerator */
    }

    /* final integer value is numerator / 2 */
    return num >> 1;
}
