/*
 * A001759  Number of permutations of [n] with n-3 sequences.
 * https://oeis.org/A001759
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = E(n+2) - 4*E(n+1) + (5-n)*E(n), with E(n) = A000111(n)
 *            (Euler zigzag numbers), derived from e.g.f. (1/2)u^3 + (11/2)u -
 *            2u^2 - (x/2)u^2 + x/2, u = sec(x)+tan(x), using u^2 = 2u' - 1
 *            and u^3 = 2u'' - u. E(n) computed via the Seidel-Entringer
 *            triangle: T(0,0)=1, T(k,0)=0, T(k,j)=T(k,j-1)+T(k...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define A1759_MAX 20010

static seq_t a1759_row[2][A1759_MAX];
static seq_t a1759_E[A1759_MAX];
static long long a1759_k = -1;

static void a1759_extend(long long K) {
    if (a1759_k < 0) {
        a1759_row[0][0] = 1;
        a1759_E[0] = 1;
        a1759_k = 0;
    }
    while (a1759_k < K) {
        seq_t *cur = a1759_row[a1759_k & 1];
        seq_t *nx = a1759_row[(a1759_k + 1) & 1];
        long long m = a1759_k + 1;
        nx[0] = 0;
        for (long long j = 1; j <= m; j++)
            nx[j] = nx[j - 1] + cur[m - j];
        a1759_E[m] = nx[m];
        a1759_k = m;
    }
}

seq_t a(idx_t n) {
    if (n < 4) return 0;
    if (n + 2 >= (idx_t)A1759_MAX) return 0;
    long long nn = (long long)n;
    a1759_extend(nn + 2);
    seq_t r = a1759_E[nn + 2] + a1759_E[nn] * 5;
    r -= a1759_E[nn + 1] * 4;
    r -= a1759_E[nn] * (seq_t)n;
    return r;
}
