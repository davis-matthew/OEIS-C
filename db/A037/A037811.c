/*
 * A037811  Number of i such that d(i) <= d(i-1), where Sum_{i=0..m} d(i)*4^i
 *          is the base-4 representation of n.
 * https://oeis.org/A037811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of indices i (i≥1) such that the i‑th base‑4 digit
 *            of n is less than or equal to the (i‑1)‑th digit
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure n ≥ 1 as per OEIS offset, but handle 0 defensively. */
    if (n == 0)
        return 0;

    idx_t tmp = n;
    /* First (least significant) digit, used as previous digit. */
    unsigned prev = (unsigned)(tmp % 4);
    tmp /= 4;

    seq_t count = 0;

    while (tmp > 0) {
        unsigned cur = (unsigned)(tmp % 4);
        if (cur <= prev)
            count += 1;
        prev = cur;
        tmp /= 4;
    }

    return count;
}
