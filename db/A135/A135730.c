/*
 * A135730  Number of steps to reach the minimum of the final cycle under
 *          iterations of the map A001281: x->3x-1 if x odd, x/2 otherwise.
 * https://oeis.org/A135730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 7120, u32 n <= 9895, u64 n <= 9895,
 *            u128 n <= 9895, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute number of iterations of map A001281
 * (x → 3x-1 if x is odd, x/2 otherwise) until the current value
 * reaches the minimal element of its final cycle (1, 5, or 17). */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t count = 0;
    while (cur != 1 && cur != 5 && cur != 17)
    {
        if (cur & 1)
            cur = 3 * cur - 1;
        else
            cur = cur / 2;
        ++count;
    }
    return count;
}
