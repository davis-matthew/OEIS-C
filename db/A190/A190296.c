/*
 * A190296  Numbers k such that the mean of the digits in k is an integer m
 *          and k is divisible by m.
 * https://oeis.org/A190296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 5740, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that the mean of the digits in k is an integer
   m and k is divisible by m.  The function scans positive integers and
   returns the nth such number. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* safety for n==0 */

    seq_t count = 0;
    seq_t candidate = 1;

    for (;;) {
        seq_t k = candidate;
        seq_t sum = 0;
        seq_t cnt = 0;
        seq_t temp = k;

        /* compute digit sum and digit count */
        while (temp != 0) {
            sum += temp % 10;
            temp /= 10;
            cnt += 1;
        }

        /* check the conditions */
        if (cnt > 0 && (sum % cnt) == 0) {
            seq_t m = sum / cnt;
            if (m > 0 && (k % m) == 0) {
                count += 1;
                if (count == n) return k;
            }
        }

        candidate += 1;
    }
}
