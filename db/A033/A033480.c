/*
 * A033480  3x + 1 sequence beginning at 15.
 * https://oeis.org/A033480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=15; a(n)=a(n-1)/2 if a(n-1) is even, otherwise 3*a(n-1)+1.
 *            For n>17 the sequence repeats with period 3: 4, 2, 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 17) {
        seq_t x = 15;
        for (idx_t i = 0; i < n; ++i) {
            if ((x % 2) == 0)
                x = x >> 1;
            else
                x = x * 3 + 1;
        }
        return x;
    } else {
        idx_t r = (n - 18) % 3;
        switch (r) {
            case 0: return 4;
            case 1: return 2;
            case 2: return 1;
        }
        return 0;   /* unreachable */
    }
}
