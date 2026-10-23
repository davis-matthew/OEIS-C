/*
 * A032947  Numbers whose base-12 representation Sum_{i=0..m} d(i)*12^i has
 *          d(i)=0 for all odd i.
 * https://oeis.org/A032947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = integers whose base‑12 digits at odd positions (counting
 *            from the most significant digit starting at 0) are zero. The
 *            sequence starts
 *            1,2,…,11,12,24,36,48,60,72,84,96,108,120,132,144,145,…
 * Range:     u8 n <= 34, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first block: single–digit numbers (base‑12 length 1) */
    if (n <= 11)            /* 1 … 11 */
        return (seq_t)n;

    /* second block: two–digit numbers (length 2) */
    idx_t m = n - 11;       /* shift after first 11 numbers */
    if (m <= 11)            /* 12,24,…,132 */
        return (seq_t)(m * 12);

    /* all other lengths (>=3) */
    m -= 11;                /* shift after length‑2 block */

    /* precompute powers of 12 up to a safe limit (here 20 exponents) */
    static seq_t pow12[20];
    static int pow12_inited = 0;
    if (!pow12_inited) {
        pow12[0] = (seq_t)1;
        for (int i = 1; i < 20; ++i)
            pow12[i] = pow12[i - 1] * (seq_t)12;
        pow12_inited = 1;
    }

    for (idx_t L = 3; ; ++L) {
        /* number of even indices in a number of length L */
        idx_t E = (L % 2 == 0) ? (L / 2) : (L / 2 + 1);

        /* count of numbers of this length: 11 * 12^(E-1) */
        idx_t exp = E - 1;                               /* exponent for 12 */
        seq_t count_seq = (seq_t)11 * pow12[exp];
        idx_t count = (idx_t)count_seq;                 /* safe: small enough */

        if (m <= count) {
            /* we are inside this block */
            idx_t t = m - 1;                            /* zero‑based index */

            /* first digit (most significant even index) : 1 .. 11 */
            idx_t div0 = (idx_t)pow12[exp];             /* 12^(E-1) as idx_t */
            idx_t pos0 = t / div0;
            seq_t d0 = (seq_t)(pos0 + 1);

            /* remaining E-1 digits (0..11) */
            seq_t digits[20];
            digits[0] = d0;
            idx_t rem = t % div0;

            for (idx_t i = 1; i < E; ++i) {
                idx_t div = (idx_t)pow12[exp - i];
                seq_t digit = rem / div;
                digits[i] = digit;
                rem = rem % div;
            }

            /* reconstruct the number from base‑12 digits,
               with odd positions being zero */
            seq_t value = 0;
            for (idx_t i = 0; i < E; ++i) {
                idx_t expWeight = L - 1 - 2 * i;        /* exponent of 12 */
                value += digits[i] * pow12[expWeight];
            }
            return value;
        }

        /* skip this block and continue */
        m -= count;
    }
}
