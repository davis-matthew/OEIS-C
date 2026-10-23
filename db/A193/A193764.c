/*
 * A193764  The number of dominoes in a largest saturated domino covering of
 *          the n X n board (n>=2).
 * https://oeis.org/A193764
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 4 - floor((n+2)^2 / 5) for n > 6, except n = 13
 *            a(2) = 2, a(3) = 6, a(4) = 12, a(5) = 18, a(6) = 26 a(13) = 129
 *            This implements the closed‑form expression for all n ≥ 2
 * Range:     u8 n <= 13, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small values explicitly */
    if (n <= 6) {
        switch ((int)n) {
            case 2: return 2;
            case 3: return 6;
            case 4: return 12;
            case 5: return 18;
            case 6: return 26;
            default: return 0;  /* not expected for n < 2 */
        }
    }

    /* Special case n = 13 */
    if (n == 13) {
        return 129;
    }

    /* General formula for n > 6, n ≠ 13 */
    seq_t nn   = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t offs = (seq_t)(n + 2);               /* n + 2 */
    seq_t sq   = offs * offs;                  /* (n+2)^2 */
    seq_t div  = sq / 5;                       /* floor((n+2)^2 / 5) */
    return nn + 4 - div;
}
