/*
 * A031290  Position of n-th 3 in A007376.
 * https://oeis.org/A031290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 6592, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) is the position of the n-th occurrence of digit 3
   in the decimal concatenation of the natural numbers 1,2,3,4,...
   The function scans integers in order, counting digits until the requested
   occurrence is reached. */
seq_t a(idx_t n)
{
    idx_t  num    = 1;      /* current natural number */
    idx_t  count3 = 0;      /* how many 3's seen */
    seq_t  pos    = 0;      /* current digit position (1‑based) */

    /* digits buffer for numbers up to 64‑bit; 20 digits is enough */
    int digits[20];

    while (count3 < n) {
        /* extract digits of num in reverse order */
        idx_t temp = num;
        int dlen = 0;
        if (temp == 0) {
            digits[dlen++] = 0;
        } else {
            while (temp > 0) {
                digits[dlen++] = temp % 10;
                temp /= 10;
            }
        }

        /* process digits from most significant to least */
        for (int i = dlen - 1; i >= 0; --i) {
            pos += 1;                 /* advance to next digit */
            if (digits[i] == 3) {     /* count a '3' */
                count3 += 1;
                if (count3 == n)
                    return pos;      /* found the n‑th 3 */
            }
        }
        num += 1;                     /* move to next natural number */
    }
    return pos; /* should never reach here for n >= 1 */
}
