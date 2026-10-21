/*
 * A092292  a(n) = 3*n + A053838(n).
 * https://oeis.org/A092292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n + A053838(n) where A053838(n) = sum of digits of n
 *            when written in base 3 (mod 3)
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the remainder of the sum of ternary digits modulo 3 */
    idx_t t = n;
    unsigned int sum_mod3 = 0;          /* holds 0,1,2 only */
    while (t) {
        sum_mod3 += t % 3;
        if (sum_mod3 >= 3)              /* keep it modulo 3 */
            sum_mod3 -= 3;
        t /= 3;
    }

    /* 3*n fits in seq_t because n is within the range for which the
       sequence terms are expected to fit into seq_t. */
    return (seq_t)n * 3 + (seq_t)sum_mod3;
}
