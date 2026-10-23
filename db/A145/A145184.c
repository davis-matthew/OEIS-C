/*
 * A145184  Continued cotangent recurrence a(n+1)=a(n)^3+3*a(n) and a(1)=10.
 * https://oeis.org/A145184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=10, a(n+1)=a(n)^3 + 3*a(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {            /* behavior below the offset is irrelevant */
        return 0;
    }
    seq_t val = 10;
    if (n == 1) {
        return val;
    }
    for (idx_t i = 2; i <= n; ++i) {
        seq_t sq   = val * val;          /* a^2 */
        seq_t cube = sq * val;           /* a^3 */
        seq_t tmp  = val * (seq_t)3;     /* 3*a */
        val = cube + tmp;                /* next term */
    }
    return val;
}
