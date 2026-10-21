/*
 * A222641  Number of iterations in Collatz (3x+1) trajectory of n to reach 1
 *          from the highest term.
 * https://oeis.org/A222641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total steps of Collatz trajectory of n minus steps to
 *            reach the first maximum value. Thus a(n) equals the steps from
 *            the maximum term to 1
 * Range:     u8 n <= 26, u16 n <= 5883, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t maxv = cur;
    idx_t step_to_max = 0;
    idx_t total_steps = 0;

    while (cur != (seq_t)1) {
        if (cur & 1)          /* odd */
            cur = cur * 3 + 1;
        else                  /* even */
            cur = cur / 2;

        total_steps++;

        if (cur > maxv) {
            maxv = cur;
            step_to_max = total_steps;
        }
    }

    return (seq_t)(total_steps - step_to_max);
}
