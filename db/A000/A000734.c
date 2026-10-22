/*
 * A000734  Boustrophedon transform of 1,1,2,4,8,16,32,...
 * https://oeis.org/A000734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Boustrophedon transform of b = 1,1,2,4,8,...: T(k,0) = b(k),
 *            T(k,j) = T(k,j-1) + T(k-1,k-j), a(n) = T(n,n), with b(0) = 1,
 *            b(k) = 2^(k-1) for k >= 1. E.g.f.: (1 + exp(2*x))*(sec(x) +
 *            tan(x))/2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define A734_MAXN 10050

static seq_t a734_buf0[A734_MAXN + 1];
static seq_t a734_buf1[A734_MAXN + 1];
static seq_t a734_res[A734_MAXN + 1];
static seq_t *a734_prev = a734_buf0;
static seq_t *a734_curr = a734_buf1;
static seq_t a734_pw;
static idx_t a734_done = 0;   /* number of rows computed so far */

seq_t a(idx_t n) {
    idx_t k, j;
    seq_t *tmp;

    if (n > A734_MAXN) return 0;

    if (a734_done == 0) {
        a734_prev[0] = 1;     /* T(0,0) = b(0) */
        a734_res[0] = 1;
        a734_pw = 1;          /* b(1) = 1 */
        a734_done = 1;
    }

    for (k = a734_done; k <= n; k++) {
        a734_curr[0] = a734_pw;   /* b(k) = 2^(k-1) */
        for (j = 1; j <= k; j++)
            a734_curr[j] = a734_curr[j - 1] + a734_prev[k - j];
        a734_res[k] = a734_curr[k];
        tmp = a734_prev; a734_prev = a734_curr; a734_curr = tmp;
        a734_pw = a734_pw * 2;
        a734_done = k + 1;
    }

    return a734_res[n];
}
