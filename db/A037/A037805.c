/*
 * A037805  Number of i such that d(i) < d(i-1), where Sum_{i=0..m} d(i)*7^i
 *          is the base-7 representation of n.
 * https://oeis.org/A037805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = # { i ≥ 1 : digit_i < digit_{i-1} } in the base‑7
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; undefined for n==0 */

    seq_t count = 0;                /* result fits comfortably in any seq_t */
    idx_t tmp = n;
    unsigned prev_digit = (unsigned)(tmp % 7);  /* d(0) */
    tmp /= 7;

    while (tmp > 0)                  /* iterate over higher digits d(1), d(2), … */
    {
        unsigned digit = (unsigned)(tmp % 7);   /* current digit d(i) */
        if (digit < prev_digit)
            ++count;                 /* d(i) < d(i-1) */
        prev_digit = digit;
        tmp /= 7;
    }
    return count;
}
