/*
 * A232503  Largest power of 2 in the Collatz (3x+1) trajectory of n.
 * https://oeis.org/A232503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   largest power of 2 appearing in the Collatz (3x+1) trajectory of
 *            n
 * Range:     u8 n <= 74, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset not defined for 0 */

    seq_t current = (seq_t)n;
    seq_t largest  = 0;

    for (;;)
    {
        /* update largest if current is a power of two */
        if (current && !(current & (current - 1)))
            if (current > largest) largest = current;

        if (current == 1)         /* reached the end of the trajectory */
            break;

        /* apply Collatz step */
        if ((current & 1) == 0)   /* even */
            current >>= 1;
        else                      /* odd */
            current = current * 3 + 1;
    }

    return largest;
}
