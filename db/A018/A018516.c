/*
 * A018516  Divisors of 555.
 * https://oeis.org/A018516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 555 */
seq_t a(idx_t n)
{
    /* 555 = 3 × 5 × 37, all divisors are the products of subsets of {3,5,37} */
    if (n < 1 || n > 8) {
        return (seq_t)0;
    }
    static const uint64_t primes[3] = {3, 5, 37};
    idx_t mask = n - 1;          /* 0..7 */
    seq_t val = 1;
    for (idx_t i = 0; i < 3; ++i) {
        if (mask & (1ULL << i)) {
            val *= (seq_t)primes[i];
        }
    }
    return val;
}
