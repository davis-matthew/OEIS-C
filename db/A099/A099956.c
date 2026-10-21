/*
 * A099956  Atomic numbers of the alkaline earth metals.
 * https://oeis.org/A099956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = atomic numbers of the alkaline earth metals. a(1)=4,
 *            a(2)=12, a(3)=20, a(4)=38, a(5)=56, a(6)=88. For n>6 the
 *            sequence has no more elements, so a(n)=0
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* static table of known atomic numbers of group 2 elements */
    static const seq_t values[6] = {4, 12, 20, 38, 56, 88};

    if (n < 1 || n > 6)
        return 0;
    return values[n - 1];
}
