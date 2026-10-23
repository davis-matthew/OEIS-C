/*
 * A126825  Ramanujan numbers (A000594) read mod 3.
 * https://oeis.org/A126825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) mod 3 if n is not divisible by 3; otherwise a(n)
 *            = 0
 * Range:     u8 n <= 2677, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 0 or 1 based on n.  As the OEIS offset is 1, n starts from 1. */
    if (n % 3 == 0) {
        return 0;
    }

    seq_t sum = 0;
    /* Enumerate divisors by iterating up to sqrt(n).  Using n/i avoids overflow. */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += (seq_t)i;
            idx_t j = n / i;
            if (j != i) {
                sum += (seq_t)j;
            }
        }
    }
    return sum % 3;
}
