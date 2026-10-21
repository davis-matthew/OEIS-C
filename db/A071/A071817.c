/*
 * A071817  Number of 3-digit numbers whose digits add up to n.
 * https://oeis.org/A071817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) = number of 3‑digit numbers whose digits add up to n
 * Range:     u8 n <= 27, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For sums beyond the possible range (1‑27) the count is zero. */
    if (n < 1 || n > 27) {
        return 0;
    }

    int sum = (int)n;          /* safe conversion: n <= 27 here */
    seq_t count = 0;

    for (int d1 = 1; d1 <= 9; ++d1) {       /* hundreds digit (non‑zero) */
        for (int d2 = 0; d2 <= 9; ++d2) {   /* tens digit */
            int d3 = sum - d1 - d2;        /* ones digit */
            if (d3 >= 0 && d3 <= 9) {
                count++;
            }
        }
    }

    return count;
}
