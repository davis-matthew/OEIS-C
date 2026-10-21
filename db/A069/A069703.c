/*
 * A069703  a(n) = n! - n^k where n^(k+1) > n! >= n^k.
 * https://oeis.org/A069703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - n^k where k is the largest integer with n^k ≤ n! <
 *            n^(k+1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n <= 2)
        return (seq_t)0;

    seq_t fact = 1;
    /* Compute factorial n! */
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Find largest k with n^k ≤ n! */
    seq_t pow = 1;
    while (1) {
        seq_t next = pow * (seq_t)n;
        if (next > fact)
            break;
        pow = next;
    }

    /* a(n) = n! - n^k */
    return fact - pow;
}
