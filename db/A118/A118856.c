/*
 * A118856  Start with 1 and repeatedly place the first digit at the end of
 *          the number and add 13.
 * https://oeis.org/A118856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=1; for n>1, a(n)= (rotate first digit of a(n‑1) to the end)
 *            + 13
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t x = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        /* find first digit and the power of 10 corresponding to its position */
        seq_t tmp = x;
        seq_t pow10 = (seq_t)1;
        while (tmp >= (seq_t)10) {
            tmp /= (seq_t)10;
            pow10 *= (seq_t)10;
        }
        seq_t first = tmp;            /* first decimal digit of x   */
        seq_t rest  = x % pow10;      /* remaining part after removing first digit */
        x = rest * (seq_t)10 + first + (seq_t)13; /* rotate and add 13 */
    }

    return x;
}
