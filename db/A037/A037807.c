/*
 * A037807  Number of i such that d(i) < d(i-1), where Sum_{i=0..m} d(i)*9^i
 *          is the base-9 representation of n.
 * https://oeis.org/A037807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of positions i such that digit d(i) < d(i-1) in
 *            the base-9 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;          /* result to return */
    idx_t t = n;              /* working copy of n */
    unsigned int prev = 9;    /* placeholder (greater than any digit 0..8) */

    while (t > 0) {
        unsigned int d = t % 9;   /* current least‑significant base‑9 digit */
        t /= 9;                   /* shift right by one digit */
        if (prev < 9) {           /* skip comparison for the first digit */
            if (d < prev) {
                count++;          /* a decrease detected */
            }
        }
        prev = d;                 /* set up for next comparison */
    }

    return count;                 /* number of decreases */
}
