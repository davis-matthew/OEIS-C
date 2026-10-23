/*
 * A189327  Number of nondecreasing arrangements of 4 numbers in 0..n with the
 *          last equal to n and each after the second equal to the sum of one
 *          or two of the preceding four.
 * https://oeis.org/A189327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) - a(n-4), a(1)=2, a(2)=6, a(3)=7, a(4)=12
 * Range:     u8 n <= 85, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A189327.  No dynamic allocation is performed;
   the recurrence is evaluated iteratively so the function works in any
   order and for any n >= 1. */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)7;
    if (n == 4) return (seq_t)12;

    /* Use a sliding window of the last four computed values. */
    seq_t v[4] = {(seq_t)2, (seq_t)6, (seq_t)7, (seq_t)12};

    for (idx_t i = 5; i <= n; ++i)
    {
        /* a(i) = 2*a(i-2) - a(i-4) */
        seq_t next = (seq_t)2 * v[2] - v[0];

        /* Shift window: drop oldest, append newest. */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = next;
    }

    return v[3];
}
