/*
 * A233999  Values of n such that numbers of the form x^2+n*y^2 for some
 *          integers x, y cannot have prime factor of 7 raised to an odd
 *          power.
 * https://oeis.org/A233999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) gives the nth positive integer such that the exponent of 7 in n is even
   and the residue n / 7^v7(n) is congruent to 1, 2, or 4 modulo 7.  The function
   enumerates integers until the required index is reached. */
seq_t a(idx_t n)
{
    idx_t target = n;
    idx_t count = 0;
    seq_t candidate = 0;
    const seq_t seven = (seq_t)7;
    while (1) {
        candidate++;
        seq_t tmp = candidate;
        idx_t exp = 0;
        while ((tmp % seven) == 0) {
            tmp /= seven;
            exp++;
        }
        if ((exp & 1) == 0) {
            seq_t r = tmp % seven;
            if (r == (seq_t)1 || r == (seq_t)2 || r == (seq_t)4) {
                count++;
                if (count == target) {
                    return candidate;
                }
            }
        }
    }
}
