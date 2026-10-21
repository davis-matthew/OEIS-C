/*
 * A089293  Sum of digits in the mixed-base enumeration system
 *          n=...d(4)d(3)d(2)d(1), where the digits satisfy 0<=d(i)<=1 if i is
 *          odd, 0<=d(i)<=2 if i is even.
 * https://oeis.org/A089293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = sum of the digits of n when written in the mixed‑base
 *            system * with alternating digit limits 0..1 and 0..2 (i.e.,
 *            bases 2 and 3, * respectively). The i‑th digit (i≥1) uses base 2
 *            if i is odd, and * base 3 if i is even
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence value for index n. */
seq_t a(idx_t n)
{
    seq_t sum  = 0;
    seq_t temp = n;        /* working copy of n */
    unsigned pos = 1;      /* digit position (1‑based) */

    while (temp > 0) {
        seq_t base = (pos & 1) ? (seq_t)2 : (seq_t)3;   /* base for this position */
        sum += temp % base;                            /* add current digit */
        temp /= base;                                  /* move to the next higher position */
        ++pos;
    }
    return sum;
}
