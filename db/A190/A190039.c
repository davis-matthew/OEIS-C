/*
 * A190039  Number of nondecreasing arrangements of n+2 numbers in 0..7 with
 *          the last equal to 7 and each after the second equal to the sum of
 *          one or two of the preceding three.
 * https://oeis.org/A190039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*n - 60 for n >= 6 Initial values used for n = 1..5:
 *            a(1)=11, a(2)=17, a(3)=22, a(4)=31, a(5)=43
 * Range:     u8 n <= 15, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values for n = 1..5 */
    static const seq_t init[5] = {11, 17, 22, 31, 43};

    if (n <= 5) {
        /* safety check: n is uint64_t, but function never called with n=0 */
        return init[(idx_t)n - 1];
    }

    /* For n >= 6 use the linear formula */
    return (seq_t)20 * (seq_t)n - (seq_t)60;
}
