/*
 * A059626  Generalized nim sum n + n + n in base 10; carryless multiplication
 *          3 X n base 10.
 * https://oeis.org/A059626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = carryless multiplication of n by 3 in base 10 (digit‑wise
 *            3*d mod 10)
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res  = 0;   /* accumulated result */
    seq_t mult = 1;   /* multiplier 10^position */

    while (n > 0) {
        uint64_t d  = n % 10ULL;            /* current decimal digit of n */
        uint64_t nd = (3ULL * d) % 10ULL;   /* digit after carryless multiplication by 3 */

        res = res + mult * (seq_t)nd;       /* add digit contribution */
        mult = mult * (seq_t)10ULL;         /* advance position */
        n = n / 10ULL;                      /* drop processed digit */
    }

    return res;
}
