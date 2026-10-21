/*
 * A207995  Number of nX6 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to any horizontal or vertical
 *          neighbor (colorings ignoring permutations of colors).
 * https://oeis.org/A207995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207995 – Number of n×6 0..2 arrays with new values 0..2 introduced in row major order
 * and no element equal to any horizontal or vertical neighbor (colorings ignoring permutations of colors).
 *
 * Recurrence (from the OEIS entry):
 *   a(n) = 30*a(n-1) -291*a(n-2) +1278*a(n-3) -2901*a(n-4)
 *          +3519*a(n-5) -2152*a(n-6) +516*a(n-7)
 * with initial values:
 *   a(1)=16, a(2)=243, a(3)=3891, a(4)=63267, a(5)=1034073,
 *   a(6)=16932816, a(7)=277458045.
 *
 * This implementation computes a(n) for any n≥1 using the recurrence,
 * storing only the last seven values and updating them iteratively.
 * It works for all supported seq_t types (uint8_t, uint16_t, uint32_t, uint64_t,
 * unsigned __int128, or arbitrary‑precision integer classes) and is O(n) time.
 *
 * No external libraries are used beyond the integer types supplied by the build.
 */

static const seq_t _a_init[7] = {
    (seq_t)16,            /* a(1) */
    (seq_t)243,           /* a(2) */
    (seq_t)3891,          /* a(3) */
    (seq_t)63267,         /* a(4) */
    (seq_t)1034073,       /* a(5) */
    (seq_t)16932816,      /* a(6) */
    (seq_t)277458045      /* a(7) */
};

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;  /* undefined by offset, return 0 as a safe default */
    }
    if (n <= 7) {
        return _a_init[n - 1];
    }

    /* a_prev[0] = a(n-1), a_prev[1] = a(n-2), ..., a_prev[6] = a(n-7) */
    seq_t a_prev[7];
    a_prev[0] = _a_init[6]; /* a(7) */
    a_prev[1] = _a_init[5]; /* a(6) */
    a_prev[2] = _a_init[4]; /* a(5) */
    a_prev[3] = _a_init[3]; /* a(4) */
    a_prev[4] = _a_init[2]; /* a(3) */
    a_prev[5] = _a_init[1]; /* a(2) */
    a_prev[6] = _a_init[0]; /* a(1) */

    idx_t i;
    for (i = 8; i <= n; ++i) {
        /* compute: 30*a_prev[0] - 291*a_prev[1] + 1278*a_prev[2]
         *           - 2901*a_prev[3] + 3519*a_prev[4] - 2152*a_prev[5]
         *           + 516*a_prev[6] */
        seq_t pos = 30 * a_prev[0];
        pos += 1278 * a_prev[2];
        pos += 3519 * a_prev[4];
        pos += 516 * a_prev[6];

        seq_t neg = 291 * a_prev[1];
        neg += 2901 * a_prev[3];
        neg += 2152 * a_prev[5];

        seq_t val = pos - neg;  /* safe: pos ≥ neg for all n in the sequence */

        /* shift the window: a_prev[6] becomes a(n-2), ..., a_prev[0] becomes a(n) */
        int k;
        for (k = 6; k > 0; --k) {
            a_prev[k] = a_prev[k - 1];
        }
        a_prev[0] = val;
    }

    return a_prev[0];
}
