/*
 * A177251  Number of permutations of [n] having no adjacent 3-cycles, i.e.,
 *          no cycles of the form (i, i+1, i+2).
 * https://oeis.org/A177251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=22, a(5)=114,
   a(n)= n*a(n-1) + a(n-3) + (n-3)*a(n-4) + 2*a(n-6)   for n >= 6. */

seq_t a(idx_t n)
{
    /* small circular buffer holding the last seven terms: a(n-6) … a(n) */
    seq_t buf[7];
    buf[0] = (seq_t)1;  /* a(0) */
    if (n == 0) return buf[0];

    buf[1] = (seq_t)1;  /* a(1) */
    if (n == 1) return buf[1];

    buf[2] = (seq_t)2;  /* a(2) */
    if (n == 2) return buf[2];

    buf[3] = (seq_t)5;  /* a(3) */
    if (n == 3) return buf[3];

    buf[4] = (seq_t)22; /* a(4) */
    if (n == 4) return buf[4];

    buf[5] = (seq_t)114;/* a(5) */
    if (n == 5) return buf[5];

    /* compute terms from 6 up to n using the recurrence */
    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t term =
            (seq_t)i * buf[(i - 1) % 7]                       /* n*a(n-1) */
          + buf[(i - 3) % 7]                                 /* a(n-3)   */
          + (seq_t)(i - 3) * buf[(i - 4) % 7]                /* (n-3)*a(n-4) */
          + (seq_t)2 * buf[(i - 6) % 7];                    /* 2*a(n-6)   */
        buf[i % 7] = term;                                  /* store a(i) */
    }
    return buf[n % 7];
}
