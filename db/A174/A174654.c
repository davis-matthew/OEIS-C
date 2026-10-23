/*
 * A174654  Partial sums of A030279.
 * https://oeis.org/A174654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} A030279(i) Recurrence for n >= 10: a(n) =
 *            12*a(n-1) - 54*a(n-2) + 115*a(n-3) - 132*a(n-4) + 108*a(n-5) -
 *            59*a(n-6) + 24*a(n-7) - 6*a(n-8) + a(n-9)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values a(1) .. a(9) */
    static const seq_t base[10] = {
        (seq_t)0,          /* unused, offset 1 */
        (seq_t)1,          /* a(1) */
        (seq_t)9,          /* a(2) */
        (seq_t)59,         /* a(3) */
        (seq_t)335,        /* a(4) */
        (seq_t)1732,       /* a(5) */
        (seq_t)8404,       /* a(6) */
        (seq_t)38969,      /* a(7) */
        (seq_t)174637,     /* a(8) */
        (seq_t)762063      /* a(9) */
    };

    if (n <= 9)
        return base[n];

    /* Use a circular buffer of the last nine terms */
    seq_t buf[9];
    buf[0] = base[1];
    buf[1] = base[2];
    buf[2] = base[3];
    buf[3] = base[4];
    buf[4] = base[5];
    buf[5] = base[6];
    buf[6] = base[7];
    buf[7] = base[8];
    buf[8] = base[9];  /* newest term */

    for (idx_t i = 10; i <= n; ++i)
    {
        /* Positive contribution */
        seq_t pos = buf[8] * (seq_t)12
                  + buf[6] * (seq_t)115
                  + buf[4] * (seq_t)108
                  + buf[2] * (seq_t)24
                  + buf[0];

        /* Negative contribution */
        seq_t neg = buf[7] * (seq_t)54
                  + buf[5] * (seq_t)132
                  + buf[3] * (seq_t)59
                  + buf[1] * (seq_t)6;

        /* Next term */
        seq_t next = pos - neg;  /* pos >= neg for all n >= 10 */

        /* Shift buffer: drop the oldest term and append next */
        for (idx_t k = 0; k < 8; ++k)
            buf[k] = buf[k + 1];
        buf[8] = next;
    }

    return buf[8];
}
