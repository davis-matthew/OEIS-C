/*
 * A167096  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 41*(a(n-1)+a(n-2)+...+a(n-12)) - 861*a(n-13) a(0)=1,
 *            a(1)=43, a(2)=1806, a(3)=75852, a(4)=3185784, a(5)=133802928,
 *            a(6)=5619722976, a(7)=236028364992, a(8)=9913191329664,
 *            a(9)=416354035845888, a(10)=17486869505527296,
 *            a(11)=734448519232146432, a(12)=30846837807750150144
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t last[13] = {0};
    for (idx_t cur = 0; cur <= n; ++cur)
    {
        seq_t p;
        if (cur == 0)
            p = 1;
        else if (cur <= 12)
            p = 2;
        else if (cur == 13)
            p = 1;
        else
            p = 0;

        seq_t sum = p;
        for (int j = 0; j < 12; ++j)
            sum += (seq_t)41 * last[j];
        sum -= (seq_t)861 * last[12];

        for (int j = 12; j > 0; --j)
            last[j] = last[j - 1];
        last[0] = sum;
    }
    return last[0];
}
