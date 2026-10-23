/*
 * A177420  a(n) = 6*a(n-1)-8*a(n-2) for n > 10; a(0)=221, a(1)=2754,
 *          a(2)=12456, a(3)=77697, a(4)=589869, a(5)=5333271, a(6)=48222198,
 *          a(7)=218509695, a(8)=3071851356, a(9)=12683673552,
 *          a(10)=51137150880.
 * https://oeis.org/A177420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 8*a(n-2) for n > 10; a(0)=221, a(1)=2754,
 *            a(2)=12456, a(3)=77697, a(4)=589869, a(5)=5333271,
 *            a(6)=48222198, a(7)=218509695, a(8)=3071851356,
 *            a(9)=12683673552, a(10)=51137150880
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 8, u64 n <= 24, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[11] = {
        (seq_t)221ULL,
        (seq_t)2754ULL,
        (seq_t)12456ULL,
        (seq_t)77697ULL,
        (seq_t)589869ULL,
        (seq_t)5333271ULL,
        (seq_t)48222198ULL,
        (seq_t)218509695ULL,
        (seq_t)3071851356ULL,
        (seq_t)12683673552ULL,
        (seq_t)51137150880ULL
    };

    if (n <= 10)
        return init[n];

    seq_t a_nm2 = init[9];   /* a(9)  */
    seq_t a_nm1 = init[10];  /* a(10) */

    for (idx_t i = 11; i <= n; ++i) {
        seq_t new_val = (seq_t)6 * a_nm1 - (seq_t)8 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = new_val;
    }

    return a_nm1;
}
