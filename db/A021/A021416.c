/*
 * A021416  Decimal expansion of 1/412.
 * https://oeis.org/A021416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 412) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (digit at position n of 1/412 in base 10) */
seq_t a(idx_t n)
{
    /* 1/412 has decimal digits obtained by repeated division.
       Starting with remainder 1, each step multiplies the
       remainder by 10, the quotient is the next digit, the
       new remainder is the remainder modulo 412. This
       algorithm works for any n ≥ 0 and uses only integer
       arithmetic. */
    const uint64_t divisor = 412ULL;
    uint64_t rem = 1ULL;          /* current remainder < divisor */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10ULL;
        seq_t digit = (seq_t)(rem / divisor);
        rem   = rem % divisor;
        if (i == n)
            return digit;
    }
    /* Control should never reach here; return 0 to silence compiler */
    return 0;
}
