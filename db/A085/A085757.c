/*
 * A085757  Differences between successive multiples of either 11 or 19.
 * https://oeis.org/A085757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = difference between successive multiples of 11 or 19, with
 *            a(0)=11
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t i     = 1;           /* next multiplier for 11 */
    idx_t j     = 1;           /* next multiplier for 19 */
    idx_t prev  = 0;           /* previous multiple in the merged list */
    idx_t next11 = 11;         /* next multiple of 11 */
    idx_t next19 = 19;         /* next multiple of 19 */
    idx_t count  = 0;          /* how many differences produced so far */

    for (;;)
    {
        idx_t next;                /* the next multiple in the merged order */

        if (next11 < next19)
        {
            next   = next11;
            next11 = (idx_t)(++i) * 11;
        }
        else if (next19 < next11)
        {
            next   = next19;
            next19 = (idx_t)(++j) * 19;
        }
        else /* equal – skip duplicate */
        {
            next   = next11;
            next11 = (idx_t)(++i) * 11;
            next19 = (idx_t)(++j) * 19;
        }

        idx_t diff = next - prev;   /* difference at this position */

        if (count == n)
            return (seq_t)diff;

        prev  = next;
        ++count;
    }
}
