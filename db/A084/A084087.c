/*
 * A084087  Numbers k not divisible by 3 such that the exponent of the highest
 *          power of 2 dividing k is even.
 * https://oeis.org/A084087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th natural number k such that (k mod 3 ≠ 0) and the
 *            exponent of the highest power of 2 dividing k is even (0, 2,
 *            4,…)
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k      = 0;

    while (1) {
        k++;
        /* skip multiples of 3 */
        if ((k % 3) == 0)
            continue;

        /* Determine parity of v₂(k).  We only need the parity,
           so toggle a flag for each trailing zero. */
        idx_t tmp = k;
        unsigned int parity = 0;   /* 0 → even, 1 → odd */
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            parity ^= 1;           /* toggle parity each time a 0 bit is removed */
        }

        if (parity == 0) {         /* even exponent of 2 */
            count++;
            if (count == n)
                return (seq_t)k;
        }
    }
}
