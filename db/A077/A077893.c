/*
 * A077893  Duplicate of A077953.
 * https://oeis.org/A077893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = floor((2^{floor(n/2)+1} + 1) / 3)
 * Range:     i8 n <= 15, i16 n <= 23, i32 n <= 23, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for k>=0,
   a(2k+1) = a(2k);
   a(2k+2) = 2*a(2k) + (-1)^{k+1} */

seq_t a(idx_t n) {
    idx_t m = n >> 1;          /* m = floor(n/2) */
    seq_t b = 1;               /* b = a(0) = 1 */
    seq_t sign = -1;           /* sign = (-1)^{k+1} for current k */

    for (idx_t i = 0; i < m; ++i) {
        b += b;      /* double b */
        b += sign;   /* add +/-1 according to the recurrence */
        sign = -sign;/* toggle sign for next step */
    }

    return b;
}
