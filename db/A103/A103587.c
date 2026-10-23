/*
 * A103587  Sequence of run lengths in A103585.
 * https://oeis.org/A103587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = repeats every 30 terms. a(1)=1, a(2)=2, a(3)=1, a(4)=2,
 *            a(5)=1, a(6)=2, a(7)=1, a(8)=1, a(9)=1, a(10)=2, a(11)=1,
 *            a(12)=2, a(13)=1, a(14)=2, a(15)=1, a(16)=1, a(17)=1, a(18)=2,
 *            a(19)=1, a(20)=2, a(21)=1, a(22)=2, a(23)=1, a(24)=1, a(25)=1,
 *            a(26)=2, a(27)=1, a(28)=2, a(29)=1, a(30)=3. For n...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t table[30] = {
        1, 2, 1, 2, 1, 2, 1, 1, 1, 2,
        1, 2, 1, 2, 1, 1, 1, 2, 1, 2,
        1, 2, 1, 1, 1, 2, 1, 2, 1, 3
    };
    idx_t i = (n - 1) % 30U;
    return table[i];
}
