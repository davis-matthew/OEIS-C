/*
 * A059857  Alternating clock-face numbers.
 * https://oeis.org/A059857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n is odd ? ((n+1)/2 + 11) : (23 - ((n/2) % 12))) % 12)
 *            + 1
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute m = sum of odd steps minus sum of even steps,
       but only its value mod 12 is relevant for a(n) */
    idx_t m = (n & 1U) ? ((n + 1U) / 2U) : (n / 2U);

    /* reduce m modulo 12 – all further calculations depend only on
       m mod 12 */
    seq_t mmod = (seq_t)(m % 12U);

    seq_t p;   /* position in 0..11 (i.e., value-1) */
    if (n & 1U) {
        /* odd step: add mmod to the starting position (11) */
        p = (11 + mmod) % 12;
    } else {
        /* even step: subtract mmod from 11, equivalently 23 - mmod */
        p = (23 - mmod) % 12;
    }

    return p + 1;   /* convert back to 1..12 */
}
