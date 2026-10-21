/*
 * A222407  Digital roots of tribonacci numbers A000073.
 * https://oeis.org/A222407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = digital_root( T_n ) where T_0 = T_1 = 0, T_2 = 1 and T_k
 *            = T_{k-1}+T_{k-2}+T_{k-3} for k >= 3. The digital root of a
 *            positive number x is (x-1)%9 + 1; the root of 0 is 0. For n >= 2
 *            the sequence repeats with period 39. This implementation uses
 *            the recurrence modulo 9 to avoid huge numbers
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first two terms are 0 */
    if (n < 2)
        return 0;

    /* sequence is periodic from n = 2 with period 39 */
    idx_t m = ((n - 2) % 39) + 2;

    /* tribonacci numbers modulo 9 */
    seq_t t0 = 0;  /* T_0 mod 9  */
    seq_t t1 = 0;  /* T_1 mod 9  */
    seq_t t2 = 1;  /* T_2 mod 9  */

    if (m == 0) return t0;   /* unreachable but keeps compiler happy */
    if (m == 1) return t1;
    if (m == 2) return t2;

    for (idx_t i = 3; i <= m; ++i) {
        seq_t sum = t0 + t1 + t2;
        t0 = t1;
        t1 = t2;
        t2 = sum % 9;        /* keep values in 0..8 */
    }

    /* convert mod-9 value to digital root:
       if the tribonacci number is positive, a root of 9 is needed
       when the modulo value is 0.  The only time the tribonacci number
       itself is 0 is at indices 0 and 1, which are handled above. */
    seq_t root = (t2 == 0) ? 9 : t2;
    return root;
}
