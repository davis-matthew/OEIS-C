/*
 * A055207  Sum of n-th powers of digits of n.
 * https://oeis.org/A055207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = sum_{digits d of n} d^n, with convention 0^0 = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle 0 specially – by definition a(0) = 1 (since 0^0 is treated as 1) */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t tmp = n;

    while (tmp > 0)
    {
        /* Extract the least‑significant decimal digit */
        unsigned digit = (unsigned)(tmp % 10);
        tmp /= 10;

        /* 0^n = 0 for n > 0, so skip zeros to save work */
        if (digit == 0)
            continue;

        /* Compute digit^n by binary exponentiation */
        seq_t power = 1;
        seq_t base  = (seq_t)digit;
        idx_t exp   = n;
        while (exp > 0)
        {
            if (exp & 1)
                power *= base;
            base *= base;
            exp >>= 1;
        }

        sum += power;
    }

    return sum;
}
