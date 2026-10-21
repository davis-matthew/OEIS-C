/*
 * A062911  Non-palindromic number and its reversal are both multiples of 8.
 * https://oeis.org/A062911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 840, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Non‑palindromic number and its reversal are both multiples of 8.
   The function searches sequentially over multiples of 8 to find the nth term
   of the OEIS A062911.  This brute‑force implementation works for any
   seq_t type that supports integer arithmetic, division by int, and
   modulo by int. */

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* Not used, but safe for index 0 */
    idx_t i = 1;                 /* Counter for multiples of 8 (i*8) */
    seq_t count = 0;             /* Number of terms found so far */
    seq_t result = 0;            /* Result to return */

    while (count < n) {
        seq_t x = (seq_t)8 * (seq_t)i;   /* Current multiple of 8 */

        /* Compute reverse of x */
        seq_t rev = 0;
        seq_t t = x;
        while (t > 0) {
            rev = rev * 10 + (t % 10);
            t /= 10;
        }

        /* Check non‑palindromic and reverse multiple of 8 */
        if (rev != x && (rev % 8) == 0) {
            count++;
            if (count == n) {
                result = x;
                break;
            }
        }
        i++;
    }
    return result;
}
