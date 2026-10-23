/*
 * A139784  a(4n+1)=2a(4n), a(4n+2)=2a(4n+1), a(4n+3)=2a(4n+2),
 *          a(4n+4)=2a(4n+3)+A007583(n).
 * https://oeis.org/A139784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+5*a(n-4)-10*a(n-5)-4*a(n-8)+8*a(n-9); initial
 *            values:
 *            a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=1,a(5)=2,a(6)=4,a(7)=8,a(8)=19
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small n explicitly */
    switch (n) {
        case 0: return (seq_t)0;
        case 1: return (seq_t)0;
        case 2: return (seq_t)0;
        case 3: return (seq_t)0;
        case 4: return (seq_t)1;
        case 5: return (seq_t)2;
        case 6: return (seq_t)4;
        case 7: return (seq_t)8;
        case 8: return (seq_t)19;
    }

    /* recurrence for n ≥ 9, using a circular buffer of size 9 */
    seq_t prev[9];
    prev[0] = (seq_t)0;
    prev[1] = (seq_t)0;
    prev[2] = (seq_t)0;
    prev[3] = (seq_t)0;
    prev[4] = (seq_t)1;
    prev[5] = (seq_t)2;
    prev[6] = (seq_t)4;
    prev[7] = (seq_t)8;
    prev[8] = (seq_t)19;

    for (idx_t i = 9; i <= n; ++i) {
        /* compute a(i) from previous values */
        seq_t val = 2 * prev[(i - 1) % 9];
        val += 5 * prev[(i - 4) % 9];
        val += 8 * prev[(i - 9) % 9];   /* (i-9)%9 == i%9 */
        val -= 10 * prev[(i - 5) % 9];
        val -= 4 * prev[(i - 8) % 9];
        prev[i % 9] = val;
    }
    return prev[n % 9];
}
