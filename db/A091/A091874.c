/*
 * A091874  Duplicate of A067633.
 * https://oeis.org/A091874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A091874: a(1)=1, a(2)=4, a(3)=13, a(4)=37, a(5)=117;
   for n≥6, a(n) is the decimal integer 3 followed by (n‑5) copies of 6 and then 7. */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 4;
    if (n == 3) return 13;
    if (n == 4) return 37;
    if (n == 5) return 117;

    idx_t t = n - 5;          /* number of 6's */
    seq_t res = 3;             /* start with the leading 3 */

    for (idx_t i = 0; i < t; ++i)
        res = res * 10 + 6;    /* append a 6 */

    res = res * 10 + 7;        /* append the final 7 */
    return res;
}
