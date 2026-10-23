/*
 * A152438  a(n) = (19^n - 1)/(9*2^(3 - 2*(n mod 2))).
 * https://oeis.org/A152438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (19^n - 1)/(9*2^(3-2*(n mod 2)))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=5, a(3)=381 */
/*               a(n) = 362*a(n-2) - 361*a(n-4)   (n ≥ 4) */

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)381;

    /* buffer of 4 last values: indices 0..3 correspond to n % 4 */
    seq_t buf[4];
    buf[0] = (seq_t)0;
    buf[1] = (seq_t)1;
    buf[2] = (seq_t)5;
    buf[3] = (seq_t)381;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t a_nm2 = buf[(i - 2) % 4];   /* a(i-2) */
        seq_t a_nm4 = buf[(i - 4) % 4];   /* a(i-4) */
        seq_t next = a_nm2 * (seq_t)362 - a_nm4 * (seq_t)361;
        buf[i % 4] = next;                /* store a(i) */
    }

    return buf[n % 4];
}
