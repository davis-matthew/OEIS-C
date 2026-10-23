/*
 * A021250  Decimal expansion of 1/246.
 * https://oeis.org/A021250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth digit of the decimal expansion of 1/246
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const unsigned long long MOD = 246ULL;      /* denominator */
    unsigned long long r = 1ULL;               /* remainder before first digit */
    seq_t digit = 0;

    /* Compute digits up to index n inclusive.
       Each iteration produces one digit and updates the remainder. */
    for (idx_t i = 0; i <= n; ++i) {
        unsigned long long t = r * 10ULL;      /* 10 * current remainder   */
        digit = (seq_t)(t / MOD);              /* next decimal digit       */
        r     = t % MOD;                       /* update remainder        */
    }
    return digit;
}
