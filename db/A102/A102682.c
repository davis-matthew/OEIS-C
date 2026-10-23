/*
 * A102682  Number of digits >= 8 in the decimal representations of all
 *          integers from 0 to n.
 * https://oeis.org/A102682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = total number of digits 8 or 9 in the decimal
 *            representations of all integers from 0 to n
 * Range:     u8 n <= 677, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t p = 1;                     /* power of 10 (1,10,100,...) */
    seq_t bign = (seq_t)n;           /* promote n to seq_t for arithmetic */

    while (p <= bign) {
        /* number of complete 10*p blocks before n */
        seq_t high = bign / (p * (seq_t)10);

        /* digit at position p in n */
        uint64_t cur = (uint64_t)((bign / p) % 10);

        /* lower part below position p */
        seq_t low = bign % p;

        /* count of digit 8 at this position */
        seq_t cnt8 = high * p;
        if (cur > 8)        /* cur == 9 */
            cnt8 += p;
        else if (cur == 8)
            cnt8 += low + 1;

        /* count of digit 9 at this position */
        seq_t cnt9 = high * p;
        if (cur == 9)
            cnt9 += low + 1;

        result += cnt8 + cnt9;

        /* prepare next higher digit place, stop to avoid overflow */
        if (p > bign / 10)
            break;
        p *= 10;
    }

    return result;
}
