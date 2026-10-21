/*
 * A215827  Number of intermediate configurations needed to be stored to
 *          calculate the number of ways in which a 9 X n grid can be tiled
 *          with triominoes.
 * https://oeis.org/A215827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=360, a(3)=4261, a(4)=20104, a(5)=44654, a(6)=70166.
 *            * For n >= 6: a(n) = 70166 + (n-6) * 25515
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle indices below offset – behaviour is irrelevant */
    if (n == 0)
        return 0;

    if (n == 1)
        return 4;
    if (n == 2)
        return 360;
    if (n == 3)
        return 4261;
    if (n == 4)
        return 20104;
    if (n == 5)
        return 44654;
    if (n == 6)
        return 70166;

    /* n >= 7 or any n >= 6 (formula works for n==6 as well) */
    return (seq_t)70166 + (seq_t)(n - 6) * (seq_t)25515;
}
