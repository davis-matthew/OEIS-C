/*
 * A021656  Decimal expansion of 1/652.
 * https://oeis.org/A021656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth decimal digit of the expansion of 1/652 (period 120). */
seq_t a(idx_t n)
{
    const seq_t MOD = 652;        /* denominator of the fraction               */
    idx_t exp = n;                /* exponent for 10^n mod 652                  */
    seq_t res = 1;                /* running product modulo MOD                 */
    seq_t base = 10 % MOD;        /* 10 modulo MOD                              */

    /* fast exponentiation of 10^n (mod MOD) */
    while (exp != 0)
    {
        if (exp & 1)
            /* promote to a larger type to avoid overflow during multiplication   */
            res = (unsigned int)(res * base) % MOD;
        base = (unsigned int)(base * base) % MOD;
        exp >>= 1;
    }

    /* compute the nth decimal digit: floor(10 * r / MOD)   */
    unsigned int r = (unsigned int)res;      /* r = 10^n mod MOD, small enough */
    seq_t digit = (seq_t)((r * 10) / MOD);    /* digit is in 0..9                 */

    return digit;
}
