/*
 * A136711  At step n the sequence lists the number of occurrences of digit (n
 *          mod k), with k>0, in all the numbers from 1 to n. Case k=7.
 * https://oeis.org/A136711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = number of occurrences of digit ((n+1) mod 7) in all
 *            integers from 1 to n+1
 * Range:     u8 n <= 755, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the target number as n+1 (since the OEIS sequence is offset by 1) */
    idx_t N = n + 1U;
    /* Digit to count */
    const unsigned d = (unsigned)(N % 7U);
    seq_t total = 0;

    for (idx_t i = 1; i <= N; ++i) {
        idx_t tmp = i;
        while (tmp > 0) {
            if ((tmp % 10U) == d)
                ++total;
            tmp /= 10U;
        }
    }
    return total;
}
