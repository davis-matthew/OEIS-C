/*
 * A224199  Number of nX3 0..3 arrays with rows unimodal and antidiagonals
 *          nondecreasing.
 * https://oeis.org/A224199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A224199:
   Number of n×3 0..3 arrays with rows unimodal and antidiagonals nondecreasing.
   Offset: 1   (so a(1) is the first term listed below)
   Empirical linear recurrence of order 7:
   a(n) = 20*a(n-1) - 70*a(n-2) + 228*a(n-3) - 276*a(n-4)
          + 392*a(n-5) - 196*a(n-6) + 144*a(n-7);
   Initial values:
   a(1)=50, a(2)=984, a(3)=16060, a(4)=263516,
   a(5)=4357084, a(6)=72105068, a(7)=1193130640.  */
seq_t a(idx_t n)
{
    /* keep the required first 7 seed values */
    static const seq_t init[8] = {
        0ULL,          /* dummy for index 0 */
        50ULL,         /* a(1) */
        984ULL,        /* a(2) */
        16060ULL,      /* a(3) */
        263516ULL,     /* a(4) */
        4357084ULL,    /* a(5) */
        72105068ULL,   /* a(6) */
        1193130640ULL  /* a(7) */
    };

    /* short‑circuit for the seed indices */
    if (n < 1)
        return 0;
    if (n <= 7)
        return init[n];

    /* sliding window of the last 7 terms; index 0 holds a(k-7)
       and index 6 holds a(k-1) during the k‑th iteration          */
    seq_t buf[7];
    buf[0] = init[1];
    buf[1] = init[2];
    buf[2] = init[3];
    buf[3] = init[4];
    buf[4] = init[5];
    buf[5] = init[6];
    buf[6] = init[7];

    for (idx_t k = 8; k <= n; ++k) {
        seq_t curr = 20 * buf[6]
                     - 70 * buf[5]
                     + 228 * buf[4]
                     - 276 * buf[3]
                     + 392 * buf[2]
                     - 196 * buf[1]
                     + 144 * buf[0];

        /* shift the window */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = curr;
    }

    return buf[6];
}
