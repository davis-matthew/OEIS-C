/*
 * A063511  a(n) = a(floor(sqrt(n))) * 2.
 * https://oeis.org/A063511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^⌈log₂(log₂(n + 1))⌉
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This can be computed by finding the smallest k such that 2^(2^k) – 1 ≥ n.
   The thresholds for k = 0 … 6 are: 1, 3, 15, 255, 65535, 4294967295, 18446744073709551615
   The answer is 2^k. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n < 1, return 0 */

    /* thresholds: 2^(2^k) – 1 for k = 0 … 6 */
    static const uint64_t thresh[] = {
        1ULL,
        3ULL,
        15ULL,
        255ULL,
        65535ULL,
        4294967295ULL,
        18446744073709551615ULL
    };

    idx_t k = 0;
    idx_t len = sizeof(thresh) / sizeof(thresh[0]);

    /* find first threshold that is not less than n */
    while (k < len && n > thresh[k]) {
        k++;
    }

    /* result = 2^k */
    return ((seq_t)1) << k;
}
