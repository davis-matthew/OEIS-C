/*
 * A049804  a(n) = n mod 4 + n mod 16 + ... + n mod 4^k, where 4^k <= n <
 *          4^(k+1).
 * https://oeis.org/A049804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0}^{⌊log_4 n⌋} (n mod 4^j)
 * Range:     u8 n <= 440, u16 n <= 9238, u32 n <= 9238, u64 n <= 9238,
 *            u128 n <= 9238, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 0;          /* accumulator for the result          */
    seq_t power = 1;          /* current power 4^j (starting with 4^0) */
    seq_t sn    = (seq_t)n;   /* cast index to seq_t for arithmetic   */

    while (power <= sn) {
        sum += sn % power;                /* add n mod 4^j */
        if (power > sn / 4) break;        /* would overflow next power      */
        power *= 4;                       /* next power 4^(j+1)             */
    }
    return sum;
}
