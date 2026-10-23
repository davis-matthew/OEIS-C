/*
 * A021411  Decimal expansion of 1/407.
 * https://oeis.org/A021411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = (R * 10) / 407 where R = 10ⁿ mod 407 (this gives the nth
 *            decimal digit of 1/407)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 10ⁿ mod 407 by fast exponentiation */
    uint64_t base  = 10ULL;
    uint64_t result = 1ULL;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % 407ULL;
        base = (base * base) % 407ULL;
        exp >>= 1;
    }

    /* the digit is floor((result * 10) / 407) */
    return (seq_t)((result * 10ULL) / 407ULL);
}
