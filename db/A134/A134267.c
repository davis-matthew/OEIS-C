/*
 * A134267  a(n) = |A090964(n+1) - A090964(n)|.
 * https://oeis.org/A134267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |A090964(n+1) - A090964(n)|
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset is 1; return 0 for n==0 as a safety guard */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;

    /* After the first term the sequence is periodic: 2,3,2,5 */
    idx_t r = (n - 2) & 3;  /* (n-2) modulo 4 */
    switch (r) {
        case 0: /* n = 2,6,10,…   -> 2 */
        case 2: /* n = 4,8,12,…   -> 2 */
            return (seq_t)2;
        case 1: /* n = 3,7,11,…   -> 3 */
            return (seq_t)3;
        case 3: /* n = 5,9,13,…   -> 5 */
            return (seq_t)5;
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
