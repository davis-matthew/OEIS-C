/*
 * A167526  Prime factors of the speed of light in a vacuum, c = 299792458
 *          (m/s).
 * https://oeis.org/A167526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Prime factors of 299792458 (m/s), in increasing order. */
seq_t a(idx_t n)
{
    static const seq_t primes[4] = { (seq_t)2, (seq_t)7, (seq_t)73, (seq_t)293339 };
    if (n == 0)
        return 0;
    if (n <= 4)
        return primes[n - 1];
    return 0;
}
