/*
 * A193765  The number of dominoes in the largest saturated domino covering of
 *          the n X n board plus one (n >= 2).
 * https://oeis.org/A193765
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 5 - floor((n+2)^2 / 5) for n > 6, n ≠ 13; a(13) =
 *            130; a(2)=3, a(3)=7, a(4)=13, a(5)=19, a(6)=27
 * Range:     u8 n <= 18, u16 n <= 286, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* below offset – undefined, return 0 */

    /* Small special cases */
    if (n <= 6) {
        switch (n) {
            case 2: return (seq_t)3;
            case 3: return (seq_t)7;
            case 4: return (seq_t)13;
            case 5: return (seq_t)19;
            case 6: return (seq_t)27;
            default: return (seq_t)0;  /* unreachable */
        }
    }

    /* Special case n = 13 */
    if (n == 13) return (seq_t)130;

    /* General formula for n >= 7, n != 13 */
    idx_t nn   = n + 2;                /* (n+2) as idx_t */
    idx_t sub  = (nn * nn) / 5;        /* floor((n+2)^2 / 5)   */
    idx_t val  = n * n + 5 - sub;      /* n^2 + 5 - sub        */
    return (seq_t)val;
}
