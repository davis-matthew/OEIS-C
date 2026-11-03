/*
 * A132971  a(2*n) = a(n), a(4*n+1) = -a(n), a(4*n+3) = 0, with a(0) = 1.
 * https://oeis.org/A132971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10922, i16 n <= 10922, i32 n <= 10922, i64 n <= 10922,
 *            i128 n <= 10922, big any n
 * Validated: standard, pass, 98 terms through n = 8854 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(2*n)     = a(n);
   a(4*n+1)   = -a(n);
   a(4*n+3)   = 0; */
seq_t a(idx_t n)
{
    seq_t sign = 1;                 /* current sign multiplier */
    while (n != 0) {
        if ((n & 1ULL) == 0) {      /* n even → a(n) = a(n/2) */
            n >>= 1;
            continue;
        }
        /* n odd */
        if ((n & 3ULL) == 1) {      /* n = 4*k+1 → a(n) = -a(k) */
            sign = -sign;
            n = (n - 1) >> 2;       /* divide by 4 */
        } else {                    /* n = 4*k+3 → a(n) = 0 */
            return 0;
        }
    }
    return sign;
}
