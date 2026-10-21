/*
 * A063012  Sum of distinct powers of 20; i.e., numbers with digits in {0,1}
 *          base 20; i.e., write n in base 2 and read as if written in base
 *          20.
 * https://oeis.org/A063012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063012: Sum of distinct powers of 20
   a(n) = Σ_{k>=0} ((n >> k) & 1) * 20^k  (a(0)=0, a(1)=1, a(2)=20, …) */
seq_t a(idx_t n) {
    seq_t value = 0;
    seq_t power = 1;          /* current 20^k as a seq_t */
    idx_t temp = n;           /* iterate through bits of n */
    while (temp) {
        if (temp & 1U)      /* LSB is 1: add corresponding power of 20 */
            value += power;
        power *= 20;        /* advance to next power of 20 */
        temp >>= 1U;        /* shift to the next bit */
    }
    return value;
}
