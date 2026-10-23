/*
 * A165979  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of reduced words of length n in a Coxeter
 *            group with 27 generators satisfying S_i^2 = (S_i S_j)^10 = I.
 *            Using the generating function
 *            (1+x)(1-x^10)/(1-26x+350x^10-325x^11) the sequence satisfies the
 *            linear recurrence a(0)=1, a(1)=27, …, a(11)=3811511582622900
 *            a(n) = 26*a(n-1) -...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 12 terms (OEIS A165979) */
    static const seq_t init[12] = {
        (seq_t)1,
        (seq_t)27,
        (seq_t)702,
        (seq_t)18252,
        (seq_t)474552,
        (seq_t)12338352,
        (seq_t)320797152,
        (seq_t)8340725952,
        (seq_t)216858874752,
        (seq_t)5638330743552,
        (seq_t)146596599332001,
        (seq_t)3811511582622900
    };

    if (n <= 11) {
        return init[n];
    }

    /* buffer holding the last 12 values a(k-12)…a(k-1) */
    seq_t buf[12];
    for (idx_t i = 0; i < 12; ++i) {
        buf[i] = init[i];
    }

    for (idx_t k = 12; k <= n; ++k) {
        seq_t val = 26 * buf[(k - 1) % 12]
                 - 350 * buf[(k - 10) % 12]
                 + 325 * buf[(k - 11) % 12];
        buf[k % 12] = val;
    }

    return buf[n % 12];
}
