/*
 * A214681  a(n) is obtained from n by removing factors of 2 and 3 that do not
 *          contribute to a factor of 6.
 * https://oeis.org/A214681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n / (2^v2(n) * 3^v3(n))) * 6^min(v2(n), v3(n))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where v2(n) and v3(n) are the 2- and 3-adic valuations of n. */

seq_t a(idx_t n)
{
    /* Count powers of 2 and 3 in n and remove them completely. */
    idx_t v2 = 0, v3 = 0;
    idx_t tmp = n;

    while ((tmp & 1ULL) == 0) {      /* while divisible by 2 */
        ++v2;
        tmp >>= 1;
    }
    while (tmp % 3ULL == 0) {        /* while divisible by 3 */
        ++v3;
        tmp /= 3;
    }

    /* Remaining factor (coprime to 6).  Compute 6^min(v2,v3). */
    idx_t v = (v2 < v3) ? v2 : v3;
    seq_t pow6 = 1;
    for (idx_t i = 0; i < v; ++i)
        pow6 *= (seq_t)6;

    /* Result: remaining factor times 6^v. */
    seq_t rem = (seq_t)tmp;   /* tmp == n / (2^v2 * 3^v3) */
    return rem * pow6;
}
