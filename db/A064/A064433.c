/*
 * A064433  Number of iterations of A064455 to reach 2 (or 1 in the case of
 *          1).
 * https://oeis.org/A064433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 5907, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064433:
   a(n) = 1 + number of iterations of the map
          f(x) = x + floor(x/2) if x is even
          f(x) = x - floor(x/2) if x is odd
   until the value reaches 2 (or 1 when n = 1).
   Example: a(4) = 6 because 4→6→9→5→3→2. */
seq_t a(idx_t n)
{
    seq_t count = 1;                    /* include starting term */
    seq_t val    = (seq_t)n;           /* work in seq_t so all arithmetic fits */

    if (val <= (seq_t)2)               /* n = 1 or 2 -> already at the limit */
        return count;

    while (val > (seq_t)2)
    {
        seq_t half = val >> 1;         /* floor(val/2) */
        if ((val & (seq_t)1) == 0)     /* even */
            val = val + half;          /* n + floor(n/2) */
        else                          /* odd */
            val = val - half;          /* n - floor(n/2) */
        count++;                      /* count the new term */
    }

    return count;
}
