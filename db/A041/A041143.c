/*
 * A041143  Denominators of continued fraction convergents to sqrt(80).
 * https://oeis.org/A041143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 17, a(3) = 18; a(n) = 18*a(n-2) -
 *            a(n-4) for n >= 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the nth term of OEIS A041143 (denominators of CF convergents to sqrt(80)). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)17;
    if (n == 3) return (seq_t)18;

    seq_t a[4];
    a[0] = (seq_t)1;   /* a(0) */
    a[1] = (seq_t)1;   /* a(1) */
    a[2] = (seq_t)17;  /* a(2) */
    a[3] = (seq_t)18;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* indices modulo 4: i-2 and i-4 */
        idx_t idx2 = (i - 2) & 3;
        idx_t idx4 = (i - 4) & 3;
        seq_t next = (seq_t)18 * a[idx2] - a[idx4];
        a[i & 3] = next;
    }

    return a[n & 3];
}
