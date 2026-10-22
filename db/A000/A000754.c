/*
 * A000754  Boustrophedon transform of odd numbers.
 * https://oeis.org/A000754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*A000111(n-k)*(2*k+1), with
 *            A000111 (Euler zigzag numbers) from the Seidel-Entringer
 *            triangle: E(0,0) = 1, E(r,0) = 0, E(r,k) = E(r,k-1) +
 *            E(r-1,r-k), A000111(r) = E(r,r)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define A754_MAXN 20005

static seq_t a754_E[A754_MAXN];
static seq_t a754_RA[A754_MAXN];
static seq_t a754_RB[A754_MAXN];
static seq_t *a754_row = a754_RA;
static seq_t *a754_oth = a754_RB;
static idx_t a754_have = 0;
static int a754_init = 0;

static void a754_extend(idx_t n)
{
    if (!a754_init) {
        a754_row[0] = (seq_t)1;
        a754_E[0] = (seq_t)1;
        a754_have = 0;
        a754_init = 1;
    }
    while (a754_have < n) {
        idx_t r = a754_have + 1;
        idx_t k;
        seq_t *t;
        a754_oth[0] = (seq_t)0;
        for (k = 1; k <= r; k++)
            a754_oth[k] = a754_oth[k - 1] + a754_row[r - k];
        a754_E[r] = a754_oth[r];
        t = a754_row;
        a754_row = a754_oth;
        a754_oth = t;
        a754_have = r;
    }
}

seq_t a(idx_t n)
{
    seq_t s = (seq_t)0;
    seq_t c = (seq_t)1;
    idx_t k;
    a754_extend(n);
    for (k = 0; k <= n; k++) {
        s += c * ((seq_t)2 * (seq_t)k + (seq_t)1) * a754_E[n - k];
        if (k < n)
            c = c * (seq_t)(n - k) / (seq_t)(k + 1);
    }
    return s;
}
