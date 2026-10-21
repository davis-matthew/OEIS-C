/*
 * A231941  Number of 2 X n 0..3 arrays with no element less than a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A231941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 86*a(n-2) + 190*a(n-3) + 319*a(n-4) -
 *            4988*a(n-5) + 10423*a(n-6) + 3869*a(n-7) - 14328*a(n-8) +
 *            132748*a(n-9) - 44748*a(n-10) - 130980*a(n-11) - 63336*a(n-12) -
 *            493560*a(n-13) - 373248*a(n-14) - 51840*a(n-15); Initial values:
 *            a(1)=16, a(2)=84, a(3)=668, a(4)=5070, a(5)=42104, a...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 1 … 15. */
    static const seq_t base[15] = {
        (seq_t)16ULL,
        (seq_t)84ULL,
        (seq_t)668ULL,
        (seq_t)5070ULL,
        (seq_t)42104ULL,
        (seq_t)326010ULL,
        (seq_t)2511252ULL,
        (seq_t)19430715ULL,
        (seq_t)152135920ULL,
        (seq_t)1190465282ULL,
        (seq_t)9289762532ULL,
        (seq_t)72404777123ULL,
        (seq_t)564819519982ULL,
        (seq_t)4409093385085ULL,
        (seq_t)34418864675001ULL
    };

    if (n == 0)
        return (seq_t)0;                /* undefined, but harmless */

    if (n <= 15)                       /* directly from table */
        return base[n - 1];

    /* Circular buffer holding last 15 computed terms. */
    seq_t buf[15];
    for (idx_t i = 0; i < 15; ++i)
        buf[i] = base[i];

    /* Compute terms n = 16 … current n. */
    for (idx_t k = 16; k <= n; ++k) {
        /* Positive contributions. */
        seq_t pos = (seq_t)16 * buf[14]     // a(k-1)
                  + (seq_t)190 * buf[12]    // a(k-3)
                  + (seq_t)319 * buf[11]    // a(k-4)
                  + (seq_t)10423 * buf[9]   // a(k-6)
                  + (seq_t)3869 * buf[8]    // a(k-7)
                  + (seq_t)132748 * buf[6]; // a(k-9)

        /* Negative contributions. */
        seq_t neg = (seq_t)86 * buf[13]      // a(k-2)
                  + (seq_t)4988 * buf[10]   // a(k-5)
                  + (seq_t)14328 * buf[7]   // a(k-8)
                  + (seq_t)44748 * buf[5]   // a(k-10)
                  + (seq_t)130980 * buf[4]  // a(k-11)
                  + (seq_t)63336 * buf[3]   // a(k-12)
                  + (seq_t)493560 * buf[2] // a(k-13)
                  + (seq_t)373248 * buf[1] // a(k-14)
                  + (seq_t)51840 * buf[0];  // a(k-15)

        seq_t next = pos - neg;            // recurrent value

        /* Rotate buffer: drop oldest, shift left, insert new. */
        for (idx_t i = 0; i < 14; ++i)
            buf[i] = buf[i + 1];
        buf[14] = next;
    }

    return buf[14];
}
