/*
 * A125382  Number of base 19 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace( A^n ) (n ≥ 0), where A is the 19×19 matrix with
 *            entries A[i][j] = 1 if |i – j| ≤ 5, otherwise 0. a(0) is defined
 *            as 1. This counts the base‑19 circular n‑digit numbers whose
 *            adjacent digits differ by at most 5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    enum { D = 19 };
    static seq_t A[D][D];
    static int init = 0;
    if (!init) {
        for (int i = 0; i < D; ++i) {
            for (int j = 0; j < D; ++j) {
                int d = i - j;
                if (d < 0) d = -d;
                A[i][j] = (d <= 5) ? (seq_t)1 : (seq_t)0;
            }
        }
        init = 1;
    }

    seq_t result[D][D];
    seq_t base[D][D];
    seq_t temp[D][D];

    /* identity matrix for result */
    for (int i = 0; i < D; ++i) {
        for (int j = 0; j < D; ++j) {
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            base[i][j]   = A[i][j];
        }
    }

    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            /* result = result * base */
            for (int i = 0; i < D; ++i) {
                for (int j = 0; j < D; ++j) {
                    seq_t s = 0;
                    for (int k = 0; k < D; ++k) {
                        s += result[i][k] * base[k][j];
                    }
                    temp[i][j] = s;
                }
            }
            for (int i = 0; i < D; ++i) {
                for (int j = 0; j < D; ++j) {
                    result[i][j] = temp[i][j];
                }
            }
        }

        /* base = base * base */
        for (int i = 0; i < D; ++i) {
            for (int j = 0; j < D; ++j) {
                seq_t s = 0;
                for (int k = 0; k < D; ++k) {
                    s += base[i][k] * base[k][j];
                }
                temp[i][j] = s;
            }
        }
        for (int i = 0; i < D; ++i) {
            for (int j = 0; j < D; ++j) {
                base[i][j] = temp[i][j];
            }
        }

        exp >>= 1;
    }

    seq_t ans = 0;
    for (int i = 0; i < D; ++i) {
        ans += result[i][i];
    }
    return ans;
}
