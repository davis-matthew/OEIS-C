/*
 * A082800  Smallest multiple of 4 beginning with n.
 * https://oeis.org/A082800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the smallest multiple of 4 that starts with the decimal
 *            * digits of n. If n is already divisible by 4 we take a(n)=n;
 *            otherwise * we append one digit. The digit needed depends only
 *            on n mod 4: * d = (4 - ((n % 4) * 2 % 4)) % 4 * and a(n) = 10 *
 *            n + d
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast to seq_t for arithmetic, n is idx_t (uint64_t) */
    if (n % 4 == 0) {
        return (seq_t)n;
    }
    /* compute the needed last digit */
    idx_t rem4 = n % 4;                     /* n mod 4 (0..3) */
    seq_t base   = (seq_t)n * 10;           /* n*10 */
    seq_t base_mod = ((seq_t)rem4 * 2) % 4; /* (n*10) mod 4 */
    seq_t add = (4 - base_mod) % 4;          /* digit to add */
    return base + add;
}
