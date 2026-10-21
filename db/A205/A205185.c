/*
 * A205185  Period 6: repeat [1, 8, 9, 8, 1, 0].
 * https://oeis.org/A205185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [1,8,9,8,1,0][(n-1) mod 6] (n ≥ 1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n > 6, a(n) = a(n-6) */
seq_t a(idx_t n)
{
    /* If called with n = 0, behaviour is irrelevant; return 0. */
    if (n == 0)
        return 0;

    /* The sequence repeats every 6 terms. */
    static const seq_t table[6] = { 1, 8, 9, 8, 1, 0 };

    /* Convert (n-1) mod 6 to an unsigned array index. */
    unsigned idx = (unsigned)((n - 1) % 6);

    return table[idx];
}
