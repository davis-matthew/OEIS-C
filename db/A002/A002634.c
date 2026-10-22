/*
 * A002634  From discordant permutations.
 * https://oeis.org/A002634
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(n) - 2*b(n-1) + b(n-2) + 2*a(n-1) + 2*a(n-2) - 2*a(n-3)
 *            + a(n-4) - 4*(-1)^n for n > 6, a(3) = -2, a(4) = 3, a(5) = 0,
 *            a(6) = 25, where b(n) = A000183(n) (sign of the (-1)^n term
 *            checked against the listed data). b(n) = s(n) + n*h(n), where:
 *            s(n) = -3*s(n-1) - 2*s(n-2) + s(n-3) + s(n-4),...
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t H[16], S[16], B[16], A[16];
static idx_t cur = 0;

static void reset_state(void) {
    int i;
    for (i = 0; i < 16; i++) {
        H[i] = (seq_t)0;
        S[i] = (seq_t)0;
        B[i] = (seq_t)0;
        A[i] = (seq_t)0;
    }
    cur = 0;
}

seq_t a(idx_t n) {
    static const int beta[10] = {0, 1, 0, -6, -4, 6, 4, -2, 0, 1};
    idx_t m;

    if (n < 3) return (seq_t)0;
    if (cur == 0 || n < cur) {
        if (cur != 0 && n + 16 > cur) return A[n & 15];
        reset_state();
    }

    for (m = cur + 1; m <= n; m++) {
        seq_t M = (seq_t)m;
        seq_t v = (M - 4) * H[(m - 1) & 15]
                + (seq_t)3 * (M - 1) * H[(m - 2) & 15]
                + (seq_t)20 * H[(m - 3) & 15]
                + ((seq_t)50 - (seq_t)7 * M) * H[(m - 4) & 15]
                - (seq_t)4 * (M - 5) * H[(m - 5) & 15]
                + ((seq_t)5 * M - 40) * H[(m - 6) & 15]
                + (seq_t)4 * (M - 7) * H[(m - 7) & 15]
                + ((seq_t)11 - M) * H[(m - 8) & 15]
                + ((seq_t)8 - M) * H[(m - 9) & 15]
                - H[(m - 10) & 15];
        seq_t sv;
        if (m <= 9) v += (seq_t)beta[m];
        H[m & 15] = v;

        if (m == 1) sv = (seq_t)-3;
        else if (m == 2) sv = (seq_t)5;
        else if (m == 3) sv = (seq_t)-6;
        else if (m == 4) sv = (seq_t)9;
        else sv = (seq_t)-3 * S[(m - 1) & 15] - (seq_t)2 * S[(m - 2) & 15]
                + S[(m - 3) & 15] + S[(m - 4) & 15];
        S[m & 15] = sv;

        B[m & 15] = sv + M * v;

        if (m == 3) A[m & 15] = (seq_t)-2;
        else if (m == 4) A[m & 15] = (seq_t)3;
        else if (m == 5) A[m & 15] = (seq_t)0;
        else if (m == 6) A[m & 15] = (seq_t)25;
        else if (m > 6) {
            seq_t r = B[m & 15] - (seq_t)2 * B[(m - 1) & 15] + B[(m - 2) & 15]
                    + (seq_t)2 * A[(m - 1) & 15] + (seq_t)2 * A[(m - 2) & 15]
                    - (seq_t)2 * A[(m - 3) & 15] + A[(m - 4) & 15];
            if (m & 1) r += 4; else r -= 4;
            A[m & 15] = r;
        }
    }
    if (n > cur) cur = n;
    return A[n & 15];
}
