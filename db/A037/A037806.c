/*
 * A037806  Number of i such that d(i) < d(i-1), where Sum_{i=0..m} d(i)*8^i
 *          is the base-8 representation of n.
 * https://oeis.org/A037806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A037806: Number of i such that d(i) < d(i-1),
   where Σ d(i)*8^i is the base‑8 representation of n. */
seq_t a(idx_t n)
{
    /* Handle the case n = 0, which is outside the offset range. */
    if (n == 0)
        return 0;

    seq_t count = 0;
    idx_t temp = n;

    /* The least significant octal digit is the comparison base. */
    seq_t prev = (seq_t)(temp % 8);
    temp /= 8;

    while (temp > 0) {
        seq_t cur = (seq_t)(temp % 8);
        if (cur < prev)
            count++;
        prev = cur;
        temp /= 8;
    }

    return count;
}
