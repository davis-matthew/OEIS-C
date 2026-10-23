/*
 * A035784  Coordination sequence for C_47 lattice.
 * https://oeis.org/A035784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^(2n) in ((1+x)/(1-x))^47
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t coeff[48];
static seq_t pow2[48];
static seq_t binom47[48];
static int coeff_initialized = 0;

static seq_t my_binom(idx_t N, idx_t K) {
    if (K > N) return 0;
    if (K > N - K) K = N - K;
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        res *= (seq_t)(N - K + i);
        res /= (seq_t)i;
    }
    return res;
}

static void init_coeff(void) {
    for (idx_t j = 0; j <= 47; ++j)
        binom47[j] = my_binom(47, j);

    pow2[0] = 1;
    for (idx_t j = 1; j <= 47; ++j)
        pow2[j] = pow2[j - 1] * 2;

    for (idx_t j = 1; j <= 47; ++j)
        coeff[j] = binom47[j] * pow2[j];

    coeff_initialized = 1;
}

seq_t a(idx_t n) {
    if (!coeff_initialized) init_coeff();

    if (n == 0)
        return 1;

    idx_t N = 2 * n - 1;
    seq_t result = 0;

    for (idx_t j = 1; j <= 47; ++j) {
        seq_t bin = my_binom(N, j - 1);
        if (bin == 0)
            break;
        result += coeff[j] * bin;
    }
    return result;
}
