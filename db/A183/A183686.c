/*
 * A183686  Number of (n+1)X8 binary arrays with every 2X2 subblock
 *          nonsingular.
 * https://oeis.org/A183686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            7*a(n-1)+57*a(n-2)-332*a(n-3)-1122*a(n-4)+5412*a(n-5)+9248*a(n-6)-35984*a(n-7)-26624*a(n-8)+82304*a(n-9)
 *            Initial values: a(1)=384, a(2)=3472, a(3)=28160, a(4)=239424,
 *            a(5)=1992320, a(6)=16748160, a(7)=140090240, a(8)=1174759296,
 *            a(9)=9838208512
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[9] = {
        (seq_t)384,
        (seq_t)3472,
        (seq_t)28160,
        (seq_t)239424,
        (seq_t)1992320,
        (seq_t)16748160,
        (seq_t)140090240,
        (seq_t)1174759296,
        (seq_t)9838208512
    };

    if (n <= 9) return init[n - 1];

    seq_t last[9];
    int i;
    for (i = 0; i < 9; ++i) last[i] = init[i];

    for (idx_t it = 10; it <= n; ++it) {
        /* a[it] = 7*a[it-1] + 57*a[it-2] - 332*a[it-3] - 1122*a[it-4]
                  + 5412*a[it-5] + 9248*a[it-6] - 35984*a[it-7] - 26624*a[it-8] + 82304*a[it-9] */
        seq_t pos = 7 * last[8] + 57 * last[7] + 5412 * last[4] + 9248 * last[3] + 82304 * last[0];
        seq_t neg = 332 * last[6] + 1122 * last[5] + 35984 * last[2] + 26624 * last[1];
        seq_t res = pos - neg;   /* guaranteed non‑negative */

        /* shift buffer: drop a[it-9], shift others left */
        for (i = 0; i < 8; ++i) last[i] = last[i + 1];
        last[8] = res;
    }

    return last[8];
}
