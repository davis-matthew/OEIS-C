/*
 * A189917  Pattern of the 97 consecutive leap years in a 400 year cycle of
 *          the Gregorian calendar.
 * https://oeis.org/A189917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = 4 * i, where i is the nth element (1‑based) of the *
 *            sequence of even multiples of 4 from 0 to 396, with the values *
 *            100, 200 and 300 removed. The sequence is periodic with period
 *            97
 * Range:     u8 n <= 62, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)        /* Offset is 1; return 0 for out‑of‑range safety */
        return 0;

    /* 0‑based index into the period of length 97 */
    idx_t k = (n - 1) % 97;

    seq_t i;
    if (k <= 23)               /* 0..23      -> i = k  */
        i = (seq_t)k;
    else if (k == 24)          /* 24         -> i = 24 */
        i = (seq_t)24;
    else if (k <= 48)          /* 25..48     -> i = k+1 */
        i = (seq_t)k + 1;
    else if (k <= 72)          /* 49..72     -> i = k+2 */
        i = (seq_t)k + 2;
    else                       /* 73..96     -> i = k+3 */
        i = (seq_t)k + 3;

    return i * (seq_t)4;
}
