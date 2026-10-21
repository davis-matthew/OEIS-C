/*
 * A239571  Number of ways to place 5 points on a triangular grid of side n so
 *          that no two of them are adjacent.
 * https://oeis.org/A239571
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (n-3)*(n-4)*(n^8+12*n^7-58*n^6-860*n^5+2141*n^4+23728*n^3-61316*n^2-244928*n+770880)/3840
 *            for n >= 3. For n >= 14 the sequence satisfies a(n) =
 *            11*a(n-1)-55*a(n-2)+165*a(n-3)-330*a(n-4)+462*a(n-5)
 *            -462*a(n-6)+330*a(n-7)-165*a(n-8)+55*a(n-9)-11*a(n-10)+a(n-11)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 21, u64 n <= 192, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* clang-format off */
static const seq_t a_base[11] = {
    /* a(3) ... a(13) */
    (seq_t)0,          /* a(3)  */
    (seq_t)0,          /* a(4)  */
    (seq_t)27,         /* a(5)  */
    (seq_t)999,        /* a(6)  */
    (seq_t)11565,      /* a(7)  */
    (seq_t)74811,      /* a(8)  */
    (seq_t)342042,     /* a(9)  */
    (seq_t)1239525,    /* a(10) */
    (seq_t)3799488,    /* a(11) */
    (seq_t)10259640,   /* a(12) */
    (seq_t)25076952    /* a(13) */
};
/* clang-format on */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0; /* undefined below the offset */

    if (n <= 13)
        return a_base[n - 3];

    /* buffer holds the last 11 computed values:
       buf[0] = a(k-11), buf[10] = a(k-1) for current k.  */
    seq_t buf[11];
    for (int i = 0; i < 11; ++i)
        buf[i] = a_base[i];

    for (idx_t k = 14; k <= n; ++k) {
        seq_t a0  = buf[10]; /* a(k-1) */
        seq_t a1  = buf[9];  /* a(k-2) */
        seq_t a2  = buf[8];  /* a(k-3) */
        seq_t a3  = buf[7];  /* a(k-4) */
        seq_t a4  = buf[6];  /* a(k-5) */
        seq_t a5  = buf[5];  /* a(k-6) */
        seq_t a6  = buf[4];  /* a(k-7) */
        seq_t a7  = buf[3];  /* a(k-8) */
        seq_t a8  = buf[2];  /* a(k-9) */
        seq_t a9  = buf[1];  /* a(k-10) */
        seq_t a10 = buf[0];  /* a(k-11) */

        seq_t next = a0 * 11
                   - a1 * 55
                   + a2 * 165
                   - a3 * 330
                   + a4 * 462
                   - a5 * 462
                   + a6 * 330
                   - a7 * 165
                   + a8 * 55
                   - a9 * 11
                   + a10;

        /* shift buffer left by one position and insert next */
        for (int i = 0; i < 10; ++i)
            buf[i] = buf[i + 1];
        buf[10] = next;
    }

    return buf[10];
}
