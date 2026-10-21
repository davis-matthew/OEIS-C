/*
 * A084904  Number of places to insert a division operator in the decimal
 *          string of n such that an integer results.
 * https://oeis.org/A084904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ k : 1 ≤ k ≤ #digits(n)-1, * (n mod 10^k) > 0, * ((n /
 *            10^k) mod (n mod 10^k)) == 0 }
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t to perform big‑int arithmetic. */
    seq_t nseq = (seq_t)n;

    seq_t count = 0;
    seq_t pow10 = (seq_t)10;             /* 10^k, starts with k = 1 */

    /* Iterate over all possible division positions between digits. */
    while (pow10 <= nseq)
    {
        seq_t right = nseq % pow10;      /* last k digits of n */
        if (right != 0)                  /* right part must be non‑zero */
        {
            seq_t left  = nseq / pow10;  /* first digits of n */
            if ((left % right) == 0)     /* left divisible by right */
                count++;
        }
        pow10 *= (seq_t)10;              /* advance to next power of 10 */
    }
    return count;
}
