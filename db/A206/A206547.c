/*
 * A206547  Positive odd numbers relatively prime to 21.
 * https://oeis.org/A206547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-12) + 42 for n≥13, with initial values a(1)=1,
 *            a(2)=5, a(3)=11, a(4)=13, a(5)=17, a(6)=19, a(7)=23, a(8)=25,
 *            a(9)=29, a(10)=31, a(11)=37, a(12)=41
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t residues[12] = {
        1, 5, 11, 13, 17, 19, 23, 25, 29, 31, 37, 41
    };
    if (n <= 12)
        return residues[n - 1];
    idx_t k = (n - 1) / 12;
    idx_t r = (n - 1) % 12;
    return residues[r] + (seq_t)42 * (seq_t)k;
}
