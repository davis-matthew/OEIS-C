/*
 * A207101  Number of 0..n arrays x(0..3) of 4 elements with each no smaller
 *          than the sum of its two previous neighbors modulo (n+1).
 * https://oeis.org/A207101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) + 2*a(n-3) - 3*a(n-4) - 6*a(n-5) + 6*a(n-7) +
 *            3*a(n-8) - 2*a(n-9) - 3*a(n-10) + a(n-12) initial values:
 *            a(1)=8, a(2)=26, a(3)=68, a(4)=140, a(5)=274, a(6)=462,
 *            a(7)=760, a(8)=1158, a(9)=1720, a(10)=2431, a(11)=3392,
 *            a(12)=4550
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values for n = 1 .. 12 (offset 1) */
    static const seq_t init[12] = {
        8, 26, 68, 140, 274, 462,
        760, 1158, 1720, 2431, 3392, 4550
    };

    if (n <= 12)
        return init[n - 1];

    /* ring buffer of size 13 to keep a(n-12) .. a(n-1) */
    seq_t buf[13];
    for (int i = 0; i < 12; ++i)
        buf[i] = init[i];

    /* compute values up to a(n) */
    for (idx_t i = 12; i < n; ++i) {
        seq_t a_n2  = buf[(i - 2) % 13];
        seq_t a_n3  = buf[(i - 3) % 13];
        seq_t a_n4  = buf[(i - 4) % 13];
        seq_t a_n5  = buf[(i - 5) % 13];
        seq_t a_n7  = buf[(i - 7) % 13];
        seq_t a_n8  = buf[(i - 8) % 13];
        seq_t a_n9  = buf[(i - 9) % 13];
        seq_t a_n10 = buf[(i - 10) % 13];
        seq_t a_n12 = buf[(i - 12) % 13];

        /* compute positive and negative parts separately */
        seq_t pos = 3 * a_n2 + 2 * a_n3 + 6 * a_n7 + 3 * a_n8 + a_n12;
        seq_t neg = 3 * a_n4 + 6 * a_n5 + 2 * a_n9 + 3 * a_n10;

        buf[i % 13] = pos - neg;
    }

    return buf[(n - 1) % 13];
}
