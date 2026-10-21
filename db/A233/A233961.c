/*
 * A233961  Number of (n+1) X (2+1) 0..3 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of all six edge and diagonal
 *          differences equal to 10.
 * https://oeis.org/A233961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) + 6*a(n-2) - 231*a(n-3) + 402*a(n-4) +
 *            1164*a(n-5) - 3026*a(n-6) - 973*a(n-7) + 5685*a(n-8) -
 *            1687*a(n-9) - 2838*a(n-10) + 1862*a(n-11) - 188*a(n-12) -
 *            88*a(n-13) + 16*a(n-14); Initial terms: a(1)=188, a(2)=864,
 *            a(3)=3976, a(4)=20564, a(5)=106056, a(6)=587472, a(7)=3219000,
 *            a(8)=1...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values a(1)..a(14) */
    const seq_t init[14] = {
        188ULL,                /* a(1)   */
        864ULL,                /* a(2)   */
        3976ULL,               /* a(3)   */
        20564ULL,              /* a(4)   */
        106056ULL,             /* a(5)   */
        587472ULL,             /* a(6)   */
        3219000ULL,            /* a(7)   */
        18340844ULL,           /* a(8)   */
        103336120ULL,          /* a(9)   */
        594474624ULL,          /* a(10)  */
        3391176148ULL,         /* a(11)  */
        19563928568ULL,        /* a(12)  */
        112234856476ULL,       /* a(13)  */
        647875877440ULL        /* a(14)  */
    };

    /* For n <= 14 we can return the pre‑computed value directly */
    if (n <= 14) return init[n - 1];

    /* Buffer holding the last 14 computed values.
       buf[0] = a(k‑13), buf[13] = a(k‑1) during iteration for a(k). */
    seq_t buf[14];
    for (idx_t i = 0; i < 14; ++i) buf[i] = init[i];

    /* Compute a(k) for k = 15 .. n using the specified recurrence. */
    for (idx_t k = 15; k <= n; ++k) {
        /* Positive contributions */
        seq_t pos = 9 * buf[13]  /* a(k-1)  */
                  + 6 * buf[12]  /* a(k-2)  */
                  + 402 * buf[10]/* a(k-4)  */
                  + 1164 * buf[9]/* a(k-5)  */
                  + 5685 * buf[6]/* a(k-8)  */
                  + 1862 * buf[3]/* a(k-11) */
                  + 16 * buf[0]; /* a(k-14) */

        /* Negative contributions */
        seq_t neg = 231 * buf[11] /* a(k-3)  */
                  + 3026 * buf[8] /* a(k-6)  */
                  + 973 * buf[7]  /* a(k-7)  */
                  + 1687 * buf[5] /* a(k-9)  */
                  + 2838 * buf[4] /* a(k-10) */
                  + 188 * buf[2]  /* a(k-12) */
                  + 88 * buf[1];  /* a(k-13) */

        seq_t next = pos - neg; /* Guaranteed to be non‑negative */

        /* Shift buffer left: discard oldest (a(k-13)) and append next */
        for (idx_t i = 0; i < 13; ++i) buf[i] = buf[i + 1];
        buf[13] = next;
    }

    /* buf[13] now holds a(n) */
    return buf[13];
}
