/*
 * A028083  Expansion of 1/((1-3x)(1-6x)(1-9x)(1-10x)).
 * https://oeis.org/A028083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (9*10^(n+3)-14*9^(n+3)+7*6^(n+3)-2*3^(n+3))/252
 *            Recurrence: a(0)=1,a(1)=28,a(2)=505,a(3)=7480,
 *            a(n)=28*a(n-1)-279*a(n-2)+1152*a(n-3)-1620*a(n-4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)28;
    if (n == 2) return (seq_t)505;
    if (n == 3) return (seq_t)7480;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)28;
    seq_t a2 = (seq_t)505;
    seq_t a3 = (seq_t)7480;
    seq_t an = a3;

    for (idx_t i = 4; i <= n; ++i) {
        an = ((seq_t)28 * a3);
        an -= ((seq_t)279 * a2);
        an += ((seq_t)1152 * a1);
        an -= ((seq_t)1620 * a0);

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
