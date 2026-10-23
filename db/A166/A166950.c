/*
 * A166950  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A166950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=11, a(2)=110, a(3)=1100, a(4)=11000, a(5)=110000,
 *            a(6)=1100000, a(7)=11000000, a(8)=110000000, a(9)=1100000000,
 *            a(10)=11000000000, a(11)=110000000000, a(12)=1100000000000.
 *            a(13)=9*(a(12)+...+a(1)) - 45*a(0) + 1. For n≥14: a(n) =
 *            9*(a(n-1)+...+a(n-12)) - 45*a(n-13)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[13] = {
        1ULL,
        11ULL,
        110ULL,
        1100ULL,
        11000ULL,
        110000ULL,
        1100000ULL,
        11000000ULL,
        110000000ULL,
        1100000000ULL,
        11000000000ULL,
        110000000000ULL,
        1100000000000ULL
    };

    if (n <= 12)
        return init[n];

    seq_t buf[13];
    for (idx_t i = 0; i < 13; ++i)
        buf[i] = init[i];

    for (idx_t k = 13; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t i = 1; i <= 12; ++i)
            sum += buf[i] * 9;
        seq_t next = sum - buf[0] * 45;
        if (k == 13)
            next += 1;              /* constant term for a(13) */
        for (idx_t i = 0; i < 12; ++i)
            buf[i] = buf[i + 1];
        buf[12] = next;
    }

    return buf[12];
}
