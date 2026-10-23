/*
 * A036990  Numbers n such that, in the binary expansion of n, reading from
 *          right to left, the number of 1's never exceeds the number of 0's.
 * https://oeis.org/A036990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A036990:
   Numbers n such that, in the binary expansion of n, reading from right to left
   the number of 1's never exceeds the number of 0's.
   a(1) = 0   (first term)
   a(n) for n ≥ 2 is the n‑th even integer that satisfies the condition. */
seq_t a(idx_t n)
{
    if (n == 1)          /* the sequence starts with 0 */
        return (seq_t)0;

    idx_t count = 1;     /* a(1) = 0 already counted */
    seq_t candidate = 2; /* start with the first even number */

    for (;; candidate += 2) {
        seq_t m = candidate;
        int diff = 0;      /* zeros minus ones in the processed suffix */

        while (m > 0) {
            if ((m & 1) == 0)
                diff++;   /* a 0 on the LSB side increases zeros */
            else
                diff--;   /* a 1 decreases zeros relative to ones */
            if (diff < 0)   /* invalid prefix – more 1's than 0's so far */
                break;
            m >>= 1;
        }

        if (diff >= 0) {       /* candidate satisfies the property */
            count++;
            if (count == n)
                return candidate;
        }
    }
}
