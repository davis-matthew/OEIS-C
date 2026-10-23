/*
 * A164331  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -990*a(n-6) + 44*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)) +
 *            p(n) where p(0)=1, p(1)=p(2)=p(3)=p(4)=p(5)=2, p(6)=1 and p(n)=0
 *            for n>6
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The first six values are given exactly:
   a(0)=1, a(1)=46, a(2)=2070, a(3)=93150, a(4)=4191750, a(5)=188628750. */

seq_t a(idx_t n)
{
    /* Handle the first six indices directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)46;
    if (n == 2) return (seq_t)2070;
    if (n == 3) return (seq_t)93150;
    if (n == 4) return (seq_t)4191750;
    if (n == 5) return (seq_t)188628750;

    /* Compute a(6) using the non‑homogeneous term p(6)=1. */
    if (n == 6)
    {
        /* Sum of a(1)..a(5). */
        seq_t sum = (seq_t)46;
        sum += (seq_t)2070;
        sum += (seq_t)93150;
        sum += (seq_t)4191750;
        sum += (seq_t)188628750;
        /* Recurrence with +p(6). */
        return (seq_t)44 * sum - (seq_t)990 * (seq_t)1 + (seq_t)1;
    }

    /* Prepare a ring buffer holding the last six values needed
       for the recurrence.  last[0] = a(current-1), ..., last[5] = a(current-6). */
    seq_t last[6];
    /* For current = 7 we need a(6)..a(1). */
    last[0] = (seq_t)8488292715ULL;   /* a(6) computed above */
    last[1] = (seq_t)188628750ULL;    /* a(5) */
    last[2] = (seq_t)4191750ULL;      /* a(4) */
    last[3] = (seq_t)93150ULL;        /* a(3) */
    last[4] = (seq_t)2070ULL;         /* a(2) */
    last[5] = (seq_t)46ULL;           /* a(1) */

    /* Current n to be computed (>=7). */
    for (idx_t i = 7; i <= n; ++i)
    {
        /* Sum of the last five values. */
        seq_t sum = last[0] + last[1] + last[2] + last[3] + last[4];
        /* Apply the homogeneous recurrence. */
        seq_t next = (seq_t)44 * sum - (seq_t)990 * last[5];

        /* Shift the ring buffer: drop the oldest a(i-6). */
        last[5] = last[4];
        last[4] = last[3];
        last[3] = last[2];
        last[2] = last[1];
        last[1] = last[0];
        last[0] = next;
    }

    return last[0];
}
