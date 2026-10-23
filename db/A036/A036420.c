/*
 * A036420  Number of ternary rooted trees with n nodes and height exactly 5.
 * https://oeis.org/A036420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = t[5][n] - t[4][n] where t[h][n] counts unordered ternary
 *            rooted trees with n nodes and height ≤ h
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 121, u64 n <= 121,
 *            u128 n <= 121, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maximum number of nodes in a ternary tree of height 5:
      (3^5 - 1)/2 = 121  */
#define HMAX   5
#define MAX_N  121

static seq_t t[HMAX + 1][MAX_N + 1] = {0};

seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        /* base case: height 1, only the single node tree */
        t[1][1] = 1;

        for (int h = 2; h <= HMAX; ++h) {
            for (int nn = 1; nn <= MAX_N; ++nn) {
                seq_t count = 0;

                /* leaf tree */
                if (nn == 1) {
                    count = 1;
                } else {
                    /* one child of size nn-1 */
                    count += t[h - 1][nn - 1];

                    /* two children : i + j = nn-1 */
                    for (int i = 1; i <= (nn - 1) / 2; ++i) {
                        int j = nn - 1 - i;
                        seq_t ti = t[h - 1][i];
                        seq_t tj = t[h - 1][j];
                        if (i == j)
                            count += ti * (ti + 1) / 2;
                        else
                            count += ti * tj;
                    }

                    /* three children : i + j + k = nn-1 */
                    if (nn >= 4) {
                        for (int i = 1; i <= nn - 3; ++i) {
                            for (int j = i; j <= nn - 2 - i; ++j) {
                                int k = nn - 1 - i - j;
                                if (k < j)            /* enforce i ≤ j ≤ k */
                                    continue;
                                seq_t ti = t[h - 1][i];
                                seq_t tj = t[h - 1][j];
                                seq_t tk = t[h - 1][k];
                                if (i == j && j == k) {               /* all equal */
                                    count += ti * (ti + 1) * (ti + 2) / 6;
                                } else if (i == j && j < k) {          /* i=j<k */
                                    count += (ti * (ti + 1) / 2) * tk;
                                } else if (i < j && j == k) {          /* i<j=k */
                                    count += ti * (tj * (tj + 1) / 2);
                                } else {                               /* all distinct */
                                    count += ti * tj * tk;
                                }
                            }
                        }
                    }
                }

                t[h][nn] = count;
            }
        }
        initialized = 1;
    }

    if (n > MAX_N)
        return (seq_t)0;

    int idx = (int)n;
    return t[HMAX][idx] - t[HMAX - 1][idx];
}
