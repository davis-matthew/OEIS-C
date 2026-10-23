/*
 * A137605  Consider the sequence: b(0) = n, and for k >= 1, b(k) = b(k-1)/2
 *          if b(k-1) is even, otherwise b(k) = n - (b(k-1)+1)/2. Then a(n) =
 *          m, where m is the smallest index such that b(m) = 1.
 * https://oeis.org/A137605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal m ≥ 0 such that the following sequence attains 1:
 *            b(0) = n; for k ≥ 1, if b(k−1) is even then b(k) = b(k−1) / 2 if
 *            b(k−1) is odd then b(k) = n − (b(k−1) + 1) / 2. a(n) is that
 *            minimal m. (Offset = 1)
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Direct simulation; values shrink roughly by a factor of 2 each
       step, so the loop completes in O(log n) iterations. */
    if (n == 1)                /* b(0) = 1 gives m = 0 */
        return (seq_t)0;

    seq_t b = (seq_t)n;        /* current value of the sequence */
    seq_t steps = 0;           /* number of steps taken so far */

    /* Iterate until the value becomes 1 */
    while (b != 1) {
        if ((b % 2) == 0) {            /* even case: divide by 2 */
            b = b / 2;
        } else {                      /* odd case: use n */
            seq_t tmp = (b + 1) / 2;   /* (b+1)/2 as integer division */
            b = (seq_t)n - tmp;        /* n – tmp */
        }
        ++steps;
    }
    return steps;
}
