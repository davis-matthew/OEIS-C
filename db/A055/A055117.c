/*
 * A055117  Base-7 complement of n (write n in base 7, then replace each digit
 *          with its base-7 negative).
 * https://oeis.org/A055117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the base‑7 complement of n: * Write n in base‑7, replace
 *            each digit d with its base‑7 negative * ( (7–d) mod 7, where d =
 *            0 gives 0 ), then interpret the new * digit string as a base‑7
 *            number. This produces the sequence * A055117
 * Range:     u8 n <= 48, u16 n <= 2400, u32 n <= 2400, u64 n <= 2400,
 *            u128 n <= 2400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation:  find the highest power of 7 not exceeding n,
 * then walk the digits from most significant to least, building
 * the complement digit by digit.  The result fits in seq_t. */

seq_t a(idx_t n)
{
    /* Find 7^k such that 7^(k-1) <= n < 7^k.  power starts at 1. */
    idx_t power = 1;
    while (power <= n / 7) {
        power *= 7;
    }

    seq_t res   = 0;    /* accumulated result in base 7 */
    idx_t rest  = n;    /* remaining part of n to process */

    while (power > 0) {
        idx_t digit = rest / power;   /* current digit (0–6) */
        rest -= digit * power;        /* remove processed part */

        /* Complement digit: 0 stays 0, others become 7–d */
        seq_t comp = digit ? (seq_t)(7 - digit) : (seq_t)0;
        res = res * 7 + comp;         /* shift and add */
        power /= 7;                   /* move to next lower digit */
    }

    return res;
}
