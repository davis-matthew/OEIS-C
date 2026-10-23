/*
 * A178362  Numbers with rounded up arithmetic mean of digits = 2.
 * https://oeis.org/A178362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with rounded up arithmetic mean of digits = 2 (search) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used; offset is 1 */
    idx_t count = 0;
    seq_t x = 2;                   /* first term of the sequence */
    for (;; ++x) {
        seq_t temp = x;
        seq_t sum  = 0;
        seq_t len  = 0;
        while (temp != 0) {
            sum += temp % (seq_t)10;
            temp /= (seq_t)10;
            ++len;
        }
        if (len == 0) len = 1;     /* safety for x==0, which never occurs */
        if (sum > len && sum <= (seq_t)2 * len) {
            ++count;
            if (count == n) return x;
        }
    }
}
