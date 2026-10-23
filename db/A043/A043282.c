/*
 * A043282  Maximal run length in base 8 representation of n.
 * https://oeis.org/A043282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximal run length of consecutive equal digits in the
 *            base‑8 representation of n (OEIS A043282)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; behaviour for n == 0 is unused. */
    if (n == 0) return 0;          /* safety guard – not part of the offset */

    /* Process the digits of n in base 8 (octal). */
    idx_t temp = n;
    unsigned char prev = (unsigned char)(temp % 8);
    temp /= 8;

    seq_t maxlen = 1;   /* at least one digit */
    seq_t curlen = 1;

    while (temp > 0) {
        unsigned char d = (unsigned char)(temp % 8);
        temp /= 8;

        if (d == prev) {
            curlen++;
        } else {
            if (curlen > maxlen) maxlen = curlen;
            curlen = 1;
            prev = d;
        }
    }

    /* account for the final run */
    if (curlen > maxlen) maxlen = curlen;
    return maxlen;
}
