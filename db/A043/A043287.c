/*
 * A043287  Maximal run length in base-13 representation of n.
 * https://oeis.org/A043287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximal run length of equal digits in the base‑13
 *            representation of n
 * Range:     u8 n <= 9817, u16 n <= 9817, u32 n <= 9817, u64 n <= 9817,
 *            u128 n <= 9817, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For safety (should not occur for signed human input) */
    if (n == 0)
        return 0;

    idx_t tmp = n;
    /* First digit (least significant in base‑13) */
    uint64_t prev = tmp % 13;
    tmp /= 13;

    seq_t maxrun = 1;
    seq_t currun = 1;

    /* Scan remaining digits */
    while (tmp > 0)
    {
        uint64_t d = tmp % 13;
        if (d == prev)
        {
            currun++;
            if (currun > maxrun)
                maxrun = currun;
        }
        else
        {
            currun = 1;
            prev = d;
        }
        tmp /= 13;
    }
    return maxrun;
}
