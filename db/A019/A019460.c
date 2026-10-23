/*
 * A019460  Add 1, multiply by 1, add 2, multiply by 2, etc., start with 2.
 * https://oeis.org/A019460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2; for k>=1: a(2k-1)=a(2k-2)+k; a(2k)=a(2k-1)*k
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 25, u64 n <= 41, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)2;                 /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        if (i & 1U)                       /* odd i: addition step */
            val += (seq_t)((i + 1) / 2);  /* add k where k=(i+1)/2 */
        else                              /* even i: multiplication step */
            val *= (seq_t)(i / 2);        /* multiply by k where k=i/2 */
    }
    return val;
}
