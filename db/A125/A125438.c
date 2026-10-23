/*
 * A125438  Number of base 26 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A[i][j] = 1 if |i-j| ≤ 7, otherwise 0.
 *            The answer a(0) is defined to be 1. The function implements the
 *            computation by pre‑computing powers of A and using binary
 *            exponentiation for any requested n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* multiply matrices: out = a * b */
static void multiply(const seq_t a[26][26],
                     const seq_t b[26][26],
                     seq_t out[26][26])
{
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 26; ++k) {
                sum += a[i][k] * b[k][j];
            }
            out[i][j] = sum;
        }
    }
}

/* pre‑compute A, A^2, A^4, …, A^(2^14) once */
static bool ready = false;
static seq_t pow_tbl[15][26][26];
static void init_pow(void)
{
    if (ready) return;

    /* A[0] */
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            int d = i > j ? i - j : j - i;
            pow_tbl[0][i][j] = (d <= 7) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* higher powers */
    seq_t tmp[26][26];
    for (int p = 1; p < 15; ++p) {
        multiply(pow_tbl[p - 1], pow_tbl[p - 1], tmp);
        for (int i = 0; i < 26; ++i)
            for (int j = 0; j < 26; ++j)
                pow_tbl[p][i][j] = tmp[i][j];
    }

    ready = true;
}

/* number of base‑26 circular n‑digit numbers with adjacent digits
   differing by at most 7.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    init_pow();

    seq_t res[26][26] = {0};
    /* identity matrix */
    for (int i = 0; i < 26; ++i) res[i][i] = (seq_t)1;

    seq_t temp[26][26];
    idx_t remaining = n;
    int bit = 0;
    while (remaining) {
        if (remaining & 1) {
            multiply(res, pow_tbl[bit], temp);
            for (int i = 0; i < 26; ++i)
                for (int j = 0; j < 26; ++j)
                    res[i][j] = temp[i][j];
        }
        remaining >>= 1;
        ++bit;
    }

    seq_t ans = 0;
    for (int i = 0; i < 26; ++i) ans += res[i][i];
    return ans;
}
