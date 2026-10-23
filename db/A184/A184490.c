/*
 * A184490  Number of (n+1)X3 0..2 arrays with every 2X2 subblock sum greater
 *          than 4.
 * https://oeis.org/A184490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 8*a(n-1)+42*a(n-2)-106*a(n-3)-213*a(n-4)+351*a(n-5)
   +186*a(n-6)-256*a(n-7)-40*a(n-8)+48*a(n-9)
   Initial values: a(1)=165, a(2)=1983, a(3)=20514, a(4)=225739,
   a(5)=2429833, a(6)=26384550, a(7)=285552669,
   a(8)=3094453927, a(9)=33517063586 */

seq_t a(idx_t n)
{
    if (n < 1)             /* undefined below offset */
        return 0;

    /* Initial terms 1..9 */
    static const seq_t init[10] = {
        0ull,               /* placeholder for index 0 */
        165ull,             /* a(1) */
        1983ull,            /* a(2) */
        20514ull,           /* a(3) */
        225739ull,          /* a(4) */
        2429833ull,         /* a(5) */
        26384550ull,        /* a(6) */
        285552669ull,       /* a(7) */
        3094453927ull,      /* a(8) */
        33517063586ull      /* a(9) */
    };

    /* Return known term if n is within the initial range. */
    if (n <= 9)
        return init[n];

    /* Circular buffer holding the last 9 computed values:
       buf[0] = a(k-9), ..., buf[8] = a(k-1) for the current k.  */
    seq_t buf[9];

    /* Initialize the buffer with terms a(1)..a(9). */
    buf[0] = init[1];
    buf[1] = init[2];
    buf[2] = init[3];
    buf[3] = init[4];
    buf[4] = init[5];
    buf[5] = init[6];
    buf[6] = init[7];
    buf[7] = init[8];
    buf[8] = init[9];

    /* Compute a(k) for k = 10 .. n using the recurrence. */
    for (idx_t k = 10; k <= n; ++k) {
        /* Positive part */
        seq_t pos = buf[8] * 8ULL + buf[7] * 42ULL
                  + buf[4] * 351ULL + buf[3] * 186ULL
                  + buf[0] * 48ULL;

        /* Negative part */
        seq_t neg = buf[6] * 106ULL + buf[5] * 213ULL
                  + buf[2] * 256ULL + buf[1] * 40ULL;

        seq_t cur = pos - neg;   /* Result for a(k) */

        /* Shift the window: drop the oldest term and add cur. */
        for (int i = 0; i < 8; ++i)
            buf[i] = buf[i + 1];
        buf[8] = cur;
    }

    /* After the loop, buf[8] holds a(n). */
    return buf[8];
}
