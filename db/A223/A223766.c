/*
 * A223766  Number of n X 4 0..1 arrays with rows, columns and antidiagonals
 *          unimodal and diagonals nondecreasing.
 * https://oeis.org/A223766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) * +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) * for n >
 *            13; a(1)=11, a(2)=56, a(3)=155, a(4)=361, a(5)=782, a(6)=1601, *
 *            a(7)=3141, a(8)=5907, a(9)=10678, a(10)=18618, a(11)=31422, *
 *            a(12)=51505, a(13)=82243
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 59, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Guard against illegal indices */
    if (n == 0) return 0;

    /* Store the first 13 terms (constant fits in 64 bits) */
    static const seq_t init[13] = {
        11ULL, 56ULL, 155ULL, 361ULL, 782ULL, 1601ULL,
        3141ULL, 5907ULL, 10678ULL, 18618ULL, 31422ULL,
        51505ULL, 82243ULL
    };

    /* n ≤ 13: return the pre‑computed value */
    if (n <= 13) {
        return init[n - 1];
    }

    /* For n > 13, compute iteratively using the recurrence. Only
     * the last nine values are needed.  Last[0] holds a(i-1),
     * Last[1] holds a(i-2), …, Last[8] holds a(i-9). */
    seq_t last[9] = {
        init[12],  /* a(13) */
        init[11],  /* a(12) */
        init[10],  /* a(11) */
        init[ 9],  /* a(10) */
        init[ 8],  /* a(9)  */
        init[ 7],  /* a(8)  */
        init[ 6],  /* a(7)  */
        init[ 5],  /* a(6)  */
        init[ 4]   /* a(5)  */
    };

    /* Compute a(14) … a(n) */
    for (idx_t i = 14; i <= n; ++i) {
        seq_t next = 9 * last[0];
        next -= 36 * last[1];
        next += 84 * last[2];
        next -= 126 * last[3];
        next += 126 * last[4];
        next -= 84 * last[5];
        next += 36 * last[6];
        next -= 9 * last[7];
        next += last[8];

        /* Shift the history for the next iteration */
        for (int k = 8; k > 0; --k) {
            last[k] = last[k - 1];
        }
        last[0] = next;
    }

    /* After the loop, last[0] holds a(n) */
    return last[0];
}
