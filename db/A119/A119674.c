/*
 * A119674  Number of states of the minimal deterministic finite automaton
 *          that accepts binary strings that represent numbers that are
 *          divisible by n.
 * https://oeis.org/A119674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n / 2^A007814(n) + A007814(n) (i.e. odd_part(n) +
 *            exponent_of_2(n) ) This follows the recurrence a(1) = 1 a(2n) =
 *            a(n) + 1 a(2n+1) = 2n + 1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n == 0 (out of OEIS range) safely */
    if (n == 0)
        return (seq_t)0;

    /* Compute the exponent of 2 in n and the odd part of n */
    idx_t odd = n;
    idx_t k   = 0;
    while ((odd & 1ULL) == 0) {
        odd >>= 1;
        ++k;
    }

    /* Convert to seq_t and add the two components */
    return (seq_t)odd + (seq_t)k;
}
