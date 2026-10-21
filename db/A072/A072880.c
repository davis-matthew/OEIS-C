/*
 * A072880  A recurrence of order 6: a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=1; a(n) =
 *          (a(n-1)^2 + a(n-2)^2 + a(n-3)^2 + a(n-4)^2 + a(n-5)^2)/a(n-6).
 * https://oeis.org/A072880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=1; For n>6: a(n) = (a(n-1)^2 +
 *            a(n-2)^2 + a(n-3)^2 + a(n-4)^2 + a(n-5)^2) / a(n-6) (OEIS
 *            A072880, offset 1)
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 10, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* buffer holds the last six terms; buf[0] = newest term,
       buf[1] the next oldest, … buf[5] = term six places back        */
    seq_t buf[6] = { (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1 };

    /* For the first six indices, just return the stored value */
    if (n <= 6)
        return buf[6 - n];   /* a(1) -> buf[5], a(2) -> buf[4], …, a(6) -> buf[0] */

    /* Iterate from 7 up to n, updating the window of six terms */
    for (idx_t i = 7; i <= n; ++i)
    {
        seq_t sum = buf[0] * buf[0]
                  + buf[1] * buf[1]
                  + buf[2] * buf[2]
                  + buf[3] * buf[3]
                  + buf[4] * buf[4];

        seq_t new_val = sum / buf[5];   /* exact division, all terms are integers */

        /* shift the buffer: drop the oldest term (buf[5]) and insert new_val */
        buf[5] = buf[4];
        buf[4] = buf[3];
        buf[3] = buf[2];
        buf[2] = buf[1];
        buf[1] = buf[0];
        buf[0] = new_val;
    }

    return buf[0];   /* newest term a(n) */
}
