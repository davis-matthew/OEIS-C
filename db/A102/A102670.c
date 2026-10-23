/*
 * A102670  Number of digits >= 2 in the decimal representations of all
 *          integers from 0 to n.
 * https://oeis.org/A102670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ∑_{j≥0, 10^j≤n} (⌊n/(10^{j+1})⌋·8·10^j + max(0,⌊(n mod
 *            10^{j+1})/10^j⌋−1)·10^j + (n mod 10^j + 1)) i.e. the number of
 *            decimal digits ≥ 2 in the representations of all integers from 0
 *            to n
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* promote index to sequence type */
    seq_t res = 0;               /* accumulator for total count */
    seq_t p = 1;                 /* 10^j at each step */

    while (p <= N) {
        seq_t cycle = p * (seq_t)10;              /* length of one full digit cycle */
        seq_t high  = N / cycle;                  /* complete cycles */
        seq_t rem   = N % cycle;                  /* remaining part after full cycles */

        seq_t count = high * (seq_t)8 * p;        /* digits ≥ 2 in full cycles */

        seq_t digit_high = rem / p;               /* value of the digit at position j in the remainder */
        if (digit_high >= (seq_t)2) {
            seq_t before = digit_high - (seq_t)2;  /* digits 2..(digit_high‑1) give full blocks */
            count += before * p;
            seq_t partial = rem % p + (seq_t)1;    /* numbers with the high digit itself */
            count += partial;
        }

        res += count;                            /* add contribution of this position */
        p *= (seq_t)10;                           /* move to next higher digit position */
    }

    return res;
}
