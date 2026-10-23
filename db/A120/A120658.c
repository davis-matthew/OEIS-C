/*
 * A120658  Expansion of 2*x*(6 + 30*x - 13*x^2 - 70*x^3)/((1-x)*(1+2*x)*(1 -
 *          4*x - 3*x^2 + 10*x^3)).
 * https://oeis.org/A120658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 3*a(n-1) + 9*a(n-2) - 15*a(n-3) - 16*a(n-4) + 20*a(n-5)
 *            for n >= 5, with a(0)=0, a(1)=12, a(2)=96, a(3)=370, a(4)=1654
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 12;
    if (n == 2) return 96;
    if (n == 3) return 370;
    if (n == 4) return 1654;

    seq_t buf[5];
    buf[0] = 0;      /* a(0) */
    buf[1] = 12;     /* a(1) */
    buf[2] = 96;     /* a(2) */
    buf[3] = 370;    /* a(3) */
    buf[4] = 1654;   /* a(4) */

    idx_t idx0 = 0, idx1 = 1, idx2 = 2, idx3 = 3, idx4 = 4;
    seq_t last = 0;

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t val = 3 * buf[idx4] + 9 * buf[idx3] + 20 * buf[idx0];
        val -= 15 * buf[idx2];
        val -= 16 * buf[idx1];

        buf[idx0] = val;
        last = val;

        idx_t tmp = idx0;
        idx0 = idx1;
        idx1 = idx2;
        idx2 = idx3;
        idx3 = idx4;
        idx4 = tmp;
    }

    return last;
}
