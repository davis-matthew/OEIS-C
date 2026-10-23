/*
 * A045797  Evenish numbers (prime to 10 and 10's digit is even).
 * https://oeis.org/A045797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = 20 * floor((n-1)/4) + [1,3,7,9][(n-1) mod 4]
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is indexed from 1; behavior for n < 1 is undefined. */
    seq_t period = (seq_t)(n - 1) / 4;
    unsigned int rem_index = (unsigned int)((n - 1) % 4);
    static const seq_t rems[4] = {1, 3, 7, 9};
    return 20 * period + rems[rem_index];
}
