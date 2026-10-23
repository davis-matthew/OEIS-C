/*
 * A041859  Denominators of continued fraction convergents to sqrt(451).
 * https://oeis.org/A041859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 92942980*a(n-10) - a(n-20) (n > 19) Initial values:
 *            a(0)=1, a(1)=4, a(2)=17, a(3)=38, a(4)=321, a(5)=6779,
 *            a(6)=54553, a(7)=115885, a(8)=518093, a(9)=2188257,
 *            a(10)=92424887, a(11)=371887805, a(12)=1579976107,
 *            a(13)=3531840019, a(14)=29834696259, a(15)=630060461458,
 *            a(16)=5070318387923, a(...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t init[20] = {
        (seq_t)1,
        (seq_t)4,
        (seq_t)17,
        (seq_t)38,
        (seq_t)321,
        (seq_t)6779,
        (seq_t)54553,
        (seq_t)115885,
        (seq_t)518093,
        (seq_t)2188257,
        (seq_t)92424887,
        (seq_t)371887805,
        (seq_t)1579976107,
        (seq_t)3531840019,
        (seq_t)29834696259,
        (seq_t)630060461458,
        (seq_t)5070318387923,
        (seq_t)10770697237304,
        (seq_t)48153107337139,
        (seq_t)203383126585860
    };

    if (n < 20)
        return init[n];

    seq_t buf[20];
    for (int i = 0; i < 20; ++i)
        buf[i] = init[i];

    seq_t coeff = (seq_t)92942980;
    seq_t result = init[0];

    for (idx_t i = 20; i <= n; ++i) {
        idx_t idx = i % 20;
        idx_t idx10 = (i - 10) % 20;
        idx_t idx20 = (i - 20) % 20;
        seq_t val = coeff * buf[idx10] - buf[idx20];
        buf[idx] = val;
        result = val;
    }
    return result;
}
