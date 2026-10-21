/*
 * A080842  Numbers in the x/3 + 1 conjecture: Repeat until 1 is reached: if x
 *          is divisible by 3 then divide by 3, otherwise add 1.
 * https://oeis.org/A080842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: concatenation of the trajectories of the map
 * f(x) = (x % 3 == 0) ? x / 3 : x + 1,
 * starting from 1,2,3,... ; the first term is 1.
 * a(n) gives the n‑th element of this concatenated list.   */
seq_t a(idx_t n)
{
    idx_t index = 1;   /* current position in the concatenated sequence */
    for (idx_t start = 1; ; ++start)
    {
        if (start == 1)          /* special case: trajectory of 1 is just {1} */
        {
            if (index == n)      /* n == 1 */
                return (seq_t)1;
            ++index;             /* move to position 2 */
            continue;
        }

        seq_t x = (seq_t)start;  /* current value in the trajectory */
        while (1)
        {
            /* apply the rule */
            x = (x % (seq_t)3 == 0) ? x / (seq_t)3 : x + (seq_t)1;

            if (index == n)      /* found the requested term */
                return x;
            ++index;

            if (x == (seq_t)1)   /* reached the end of this trajectory */
                break;
        }
    }
}
