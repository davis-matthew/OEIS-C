/*
 * A164771  Numbers k such that the average digit of k^2 is 1.
 * https://oeis.org/A164771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A164771 (numbers k such that the average
   decimal digit of k² equals 1) by brute‑force search over k, checking
   that digit_sum(k²) equals the number of digits of k². */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */
    seq_t target = (seq_t)n;         /* number of terms to find */
    seq_t count  = 0;                /* terms found so far */
    seq_t k      = 1;                /* current candidate */

    for (;;) {
        seq_t sq = k * k;           /* k² */
        seq_t t  = sq;
        unsigned long long sum   = 0;   /* sum of digits of k² */
        unsigned long long digs  = 0;   /* number of digits of k² */

        /* Compute sum of digits and digit count */
        while (t != 0) {
            sum += (unsigned)(t % 10);
            t   /= 10;
            ++digs;
        }

        if (sum == digs) {              /* condition holds */
            ++count;
            if (count == target) {
                return k;                /* nth term found */
            }
        }
        ++k;                              /* try next k */
    }
}
