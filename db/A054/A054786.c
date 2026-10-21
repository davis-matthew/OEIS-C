/*
 * A054786  Earliest sequence with a(a(n)) = 6n.
 * https://oeis.org/A054786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(12n)=6*a(2n) , a(12n+6)=6*a(2n+1) ; all other values are
 *            explicit
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _A054786_base[12] = {
    (seq_t)0, (seq_t)2, (seq_t)6, (seq_t)4,
    (seq_t)18, (seq_t)7, (seq_t)12, (seq_t)30,
    (seq_t)9, (seq_t)48, (seq_t)11, (seq_t)60
};

seq_t a(idx_t n)
{
    if (n < 12u)
        return _A054786_base[n];

    idx_t r = n % 12u;
    seq_t m = (seq_t)(n / 12u);            // floor(n/12)

    switch (r) {
        case 0:
        case 6:
            return 6 * a(n / 6u);
        case 1:
            return 12 * m + 2;
        case 2:
            return 72 * m + 6;
        case 3:
            return 12 * m + 4;
        case 4:
            return 72 * m + 18;
        case 5:
            return 12 * m + 7;
        case 7:
            return 72 * m + 30;
        case 8:
            return 12 * m + 9;
        case 9:
            return 72 * m + 48;
        case 10:
            return 12 * m + 11;
        case 11:
            return 72 * m + 60;
    }
    return 0;   /* unreachable */
}
