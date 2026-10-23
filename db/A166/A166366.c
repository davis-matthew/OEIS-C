/*
 * A166366  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=8, a(2)=56, a(3)=392, a(4)=2744, a(5)=19208,
 *            a(6)=134456, a(7)=941192, a(8)=6588344, a(9)=46118408,
 *            a(10)=322828856; For n>=11: a(n)=6*(a(n-1)+a(n-2)+...+a(n-10)) -
 *            21*a(n-11) + (n==11?1:0)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial 11 terms */
    static const seq_t base[11] = {
        (seq_t)1,
        (seq_t)8,
        (seq_t)56,
        (seq_t)392,
        (seq_t)2744,
        (seq_t)19208,
        (seq_t)134456,
        (seq_t)941192,
        (seq_t)6588344,
        (seq_t)46118408,
        (seq_t)322828856
    };

    if (n < 11)
        return base[n];

    /* window of the last 11 values: last[0]=a(cur-11), ..., last[10]=a(cur-1) */
    seq_t last[11];
    for (idx_t i = 0; i < 11; ++i)
        last[i] = base[i];

    seq_t new_val, sum;
    for (idx_t cur = 11; cur <= n; ++cur) {
        /* sum of a(cur-1)...a(cur-10) */
        sum = last[1];
        for (idx_t i = 2; i < 11; ++i)
            sum += last[i];

        new_val = (seq_t)6 * sum - (seq_t)21 * last[0];
        if (cur == 11)
            new_val += (seq_t)1;          /* RHS term for n=11 */

        /* shift window */
        for (idx_t i = 0; i < 10; ++i)
            last[i] = last[i + 1];
        last[10] = new_val;
    }

    return last[10];
}
