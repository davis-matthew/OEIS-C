/*
 * A038574  Write n in ternary, sort digits into increasing order.
 * https://oeis.org/A038574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = write n in base 3, sort its digits in increasing order,
 *            then read the resulting string as a base‑3 number
 * Range:     u8 n <= 363, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count the digits 0, 1, 2 in the ternary representation of n. */
    idx_t cnt[3] = {0, 0, 0};
    idx_t tmp = n;
    while (tmp != 0) {
        cnt[tmp % 3]++;   /* tmp % 3 is 0, 1, or 2 */
        tmp /= 3;
    }

    /* If n == 0, the loop above skipped; the value 0 is still correct. */

    /* Build the sorted number: first all 0's, then 1's, then 2's.
       Leading zeros have no effect on the value. */
    seq_t result = 0;

    /* zeros contribute nothing, but we must iterate over them to
       preserve the correct length of the number when n has only zeros
       (which only happens for n == 0). */
    for (idx_t i = 0; i < cnt[0]; ++i)
        result = result * 3;             /* + 0 */

    for (idx_t i = 0; i < cnt[1]; ++i)
        result = result * 3 + 1;

    for (idx_t i = 0; i < cnt[2]; ++i)
        result = result * 3 + 2;

    return result;
}
