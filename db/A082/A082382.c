/*
 * A082382  Table which contains in row n the track of n->A003132(n) until
 *          reaching 1 or 4.
 * https://oeis.org/A082382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, tabf
 * Formula:   a(n) is the n-th element of the flattened triangular table whose
 *            row k consists of the sequence starting at k and repeatedly
 *            applying the sum‑of‑squares‑of‑digits function until a terminal
 *            value 1 or 4 is reached. For the special rows starting at 1 or 4
 *            the table is extended until 1 or 4 is encou...
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;           /* how many table elements have been seen */

    for (idx_t start = 1; ; ++start)   /* generate rows one by one   */
    {
        seq_t val = (seq_t)start;        /* first element of the row */
        ++count;
        if (count == n) return val;

        /* iterate the sum‑of‑squares‑of‑digits until 1 or 4 is reached */
        while (1)
        {
            /* compute next = sum of squares of decimal digits of val  */
            seq_t next = 0;
            seq_t tmp   = val;
            while (tmp > 0)
            {
                seq_t d = tmp % 10;
                next += d * d;
                tmp  /= 10;
            }

            ++count;
            if (count == n) return next;

            /* stop at the first occurrence of 1 or 4 */
            if (next == 1 || next == 4)
                break;

            val = next;
        }
    }
}
