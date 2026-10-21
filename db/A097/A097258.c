/*
 * A097258  Numbers whose set of base 12 digits is {0,B}, where B base 12 = 11
 *          base 10.
 * https://oeis.org/A097258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(2n) = 12*a(n), a(2n+1) = a(2n) + 11, a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = 11 * Σ (bit_i of n) * 12^i   (each bit contributes a 12‑power) */
seq_t a(idx_t n)
{
    seq_t sum   = 0;          /* Σ 12^i for set bits of n */
    seq_t power = 1;          /* 12^i, starting with i = 0 */
    while (n)
    {
        if (n & 1)
            sum += power;     /* add 12^i if the ith bit is 1 */
        n >>= 1;              /* move to the next bit */
        if (n)                /* avoid unnecessary multiplication on final iteration */
            power *= (seq_t)12;
    }
    return sum * (seq_t)11;   /* each digit is 11, not 1 */
}
