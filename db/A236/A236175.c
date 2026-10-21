/*
 * A236175  Prime gap pattern of compacting Eratosthenes sieve for prime(4) =
 *          7.
 * https://oeis.org/A236175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A359632(n) - 1. Recurrence: a(n+8) = a(n)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence repeats every 8 terms:
       11, 6, 3, 6, 3, 6, 11, 2 */
    static const seq_t pat[8] = {11, 6, 3, 6, 3, 6, 11, 2};
    /* Compute (n-1) modulo 8; n >= 1 by spec. */
    return pat[(n - 1) % 8];
}
