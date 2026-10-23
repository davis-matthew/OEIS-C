/*
 * A041966  Numerators of continued fraction convergents to sqrt(506).
 * https://oeis.org/A041966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 90*a(n-2) - a(n-4) (n≥4), a(0)=22, a(1)=45, a(2)=2002,
 *            a(3)=4049
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A041966: */
/*  Numerators of continued fraction convergents to sqrt(506). */
seq_t a(idx_t n)
{
    /* Initial four terms */
    const seq_t init[4] = { (seq_t)22,
                            (seq_t)45,
                            (seq_t)2002,
                            (seq_t)4049 };

    /* Handle indices below the offset. */
    if (n < 4)
        return init[n];

    /* Circular buffer to hold the last four computed values. */
    seq_t vals[4];
    vals[0] = init[0];
    vals[1] = init[1];
    vals[2] = init[2];
    vals[3] = init[3];

    const seq_t C90 = (seq_t)90;

    /* Iterate from n = 4 up to the requested index. */
    for (idx_t i = 4; i <= n; ++i)
    {
        /* Indices modulo 4, using bitwise & because 4 is a power of two. */
        seq_t prev2 = vals[(i - 2) & 3];
        seq_t prev4 = vals[(i - 4) & 3];

        /* Recurrence: a(i) = 90*a(i-2) - a(i-4). */
        seq_t next = C90 * prev2 - prev4;

        vals[i & 3] = next;
    }

    /* The desired term resides at index n modulo 4. */
    return vals[n & 3];
}
