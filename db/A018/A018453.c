/*
 * A018453  Divisors of 434.
 * https://oeis.org/A018453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the nth divisor of 434 in ascending order (1, 2, 7, 14,
 *            31, 62, 217, 434)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    const seq_t primes[] = {2, 7, 31};
    const idx_t max_n = 8;
    if (n > max_n) return 0;
    seq_t result = 1;
    idx_t k = n - 1;
    for (idx_t i = 0; i < sizeof(primes)/sizeof(primes[0]); ++i) {
        if (k & ((idx_t)1 << i))
            result *= primes[i];
    }
    return result;
}
