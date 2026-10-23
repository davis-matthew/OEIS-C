/*
 * A021934  Decimal expansion of 1/930.
 * https://oeis.org/A021934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is the n‑th decimal digit of 1/930.  The decimal
   expansion of 1/930 is 0.00107526881720430107526…, i.e.
   a(0)=0, and for n≥1 the period of the remaining digits is 15:
   0,1,0,7,5,2,6,8,8,1,7,2,0,4,3 repeating endlessly.  */
seq_t a(idx_t n)
{
    /* Periodic table of the decimal digits of 1/93. */
    static unsigned char period[15];
    static int ready = 0;

    if (!ready) {
        idx_t rem = 1;                /* remainder of 1/93 */
        for (int i = 0; i < 15; ++i) {
            rem = rem * 10;
            period[i] = rem / 93;     /* next decimal digit           */
            rem = rem % 93;           /* update remainder             */
        }
        ready = 1;
    }

    if (n == 0) /* leading zero before the 1/93 part */
        return 0;

    idx_t p = (n - 1) % 15;          /* index into the period  */
    return (seq_t)period[p];         /* cast to the required return type */
}
