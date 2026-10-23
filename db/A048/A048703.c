/*
 * A048703  Numbers which in base 4 are palindromes and have an even number of
 *          digits.
 * https://oeis.org/A048703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenating the base‑4 digits of n with their reverse.
 *            * a(n) = n * 4^k + rev_base4(n), * where k is the number of
 *            base‑4 digits of n and rev_base4(n) * reverses those digits.
 *            a(0)=0
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n == 0 – the loop below also produces 0, but this
       keeps the code a little clearer and avoids an extra division by 4
       when n is 0. */
    if (n == 0) return 0;

    /* Convert n to base‑4 and count its digits. */
    seq_t b   = (seq_t)n;          /* the original number, also the left part */
    idx_t k   = 0;                 /* number of base‑4 digits of n */
    seq_t tmp = b;
    while (tmp) {
        tmp /= 4;
        ++k;
    }

    /* Reverse the base‑4 digits. */
    seq_t rev = 0;
    tmp = b;
    while (tmp) {
        rev = rev * 4 + (tmp % 4); /* tmp % 4 is the least‑significant digit */
        tmp /= 4;
    }

    /* Compute 4^k (the shift required to append the reversed digits). */
    seq_t power = 1;
    for (idx_t i = 0; i < k; ++i) {
        power *= 4;
    }

    /* Form the palindrome in decimal. */
    return b * power + rev;
}
