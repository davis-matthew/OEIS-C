/*
 * A080464  Product of the two numbers formed by alternate digits of n.
 * https://oeis.org/A080464
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = product of two numbers formed by alternate digits of n.
 *            First number consists of digits at positions 0,2,4,… (most
 *            significant digit counted as position 0); second number consists
 *            of digits at positions 1,3,5,…. a(n) = high * low where high is
 *            the number formed from even‑indexed digits and...
 * Range:     u8 n <= 298, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 10 the sequence is undefined; return 0. */
    if (n < 10) return 0;

    seq_t high = 0;                 /* number from even positions   */
    seq_t low  = 0;                 /* number from odd positions    */
    idx_t toggle = 0;               /* 0 → even position, 1 → odd  */

    /* Find the most significant power of 10 not exceeding n. */
    idx_t power = 1;
    idx_t tmp = n;
    while (tmp >= 10) {
        power *= 10;
        tmp  /= 10;
    }

    /* Extract digits from most significant to least and build the two
       numbers. */
    while (power > 0) {
        /* Digit at current position (unsigned 0‑9). */
        seq_t digit = (seq_t)((n / power) % 10ULL);

        if (toggle == 0) {
            high = high * 10 + digit;  /* even position */
            toggle = 1;
        } else {
            low = low * 10 + digit;   /* odd position  */
            toggle = 0;
        }
        power /= 10;
    }

    /* Return the product (may use arbitrary precision if seq_t is a
       big integer class). */
    return high * low;
}
