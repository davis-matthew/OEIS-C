/*
 * A209096  Number of n X 4 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A209096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A209096
   Empirical recurrence (valid for n > 6):
   a(n) = 49*a(n-1) - 356*a(n-2) + 705*a(n-3) - 425*a(n-4) + 64*a(n-5)
   Initial terms:
   a(1)=14, a(2)=520, a(3)=20928, a(4)=849548,
   a(5)=34538488, a(6)=1404480904 */

/* No dynamic allocation; compute iteratively for each call. */
seq_t a(idx_t n)
{
    /* Return 0 for an undefined index 0 (offset 1). */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Handle the six pre‑computed values directly. */
    switch (n)
    {
        case 1: return (seq_t)14;
        case 2: return (seq_t)520;
        case 3: return (seq_t)20928;
        case 4: return (seq_t)849548;
        case 5: return (seq_t)34538488;
        case 6: return (seq_t)1404480904;
        default: /* compute for n > 6 */
            break;
    }

    /* a(6), a(5), a(4), a(3), a(2) are needed to seed the recurrence. */
    seq_t last[5];
    last[0] = (seq_t)1404480904;  /* a(6)  = a(n-1) when n = 7   */
    last[1] = (seq_t)34538488;    /* a(5)  = a(n-2)           */
    last[2] = (seq_t)849548;      /* a(4)  = a(n-3)           */
    last[3] = (seq_t)20928;       /* a(3)  = a(n-4)           */
    last[4] = (seq_t)520;         /* a(2)  = a(n-5)           */

    seq_t cur = 0;  /* will hold the current a(n) */
    for (idx_t i = 7; i <= n; ++i)
    {
        /* Compute the positive and negative contributions separately
           to avoid unsigned underflow (the recurrence guarantees
           the positive sum is always ≥ the negative sum). */
        seq_t pos = (seq_t)49 * last[0] + (seq_t)705 * last[2]
                    + (seq_t)64 * last[4];
        seq_t neg = (seq_t)356 * last[1] + (seq_t)425 * last[3];
        cur = pos - neg;

        /* Shift the buffer for the next iteration. */
        last[4] = last[3];
        last[3] = last[2];
        last[2] = last[1];
        last[1] = last[0];
        last[0] = cur;
    }

    return cur;
}
