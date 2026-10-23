/*
 * A035834  Coordination sequence for lattice D*_98 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..98} 2^k * binom(98,k) * binom(n-1, k-1) + 2^98
 *            * binom(n+48, 97); a(0)=1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom98[99];
static seq_t pow2k[99];
static int initialized = 0;

seq_t a(idx_t n) {
    /* Handle offset */
    if (n == 0)
        return 1;

    /* Initialize static tables once */
    if (!initialized) {
        binom98[0] = 1;
        for (idx_t k = 1; k <= 98; ++k)
            binom98[k] = binom98[k - 1] * (98 - k + 1) / k;   /* binom(98,k) */

        pow2k[0] = 1;
        for (idx_t k = 1; k <= 98; ++k)
            pow2k[k] = pow2k[k - 1] * 2;                      /* 2^k */
        initialized = 1;
    }

    seq_t sum = 0;

    idx_t N_minus1 = n - 1;
    idx_t limit = (n <= 98) ? n : 98;  /* k runs up to min(98, n) */

    seq_t binom_k_minus_1 = 1;  /* binom(N_minus1, 0) */

    for (idx_t k = 1; k <= limit; ++k) {
        /* term = 2^k * binom(98,k) * binom(N_minus1, k-1) */
        seq_t term = binom_k_minus_1 * binom98[k];
        term = term * pow2k[k];
        sum += term;

        /* Update binom(N_minus1, k) for next iteration */
        if (k == limit)
            break;
        seq_t mult = binom_k_minus_1 * (N_minus1 - k + 1);
        binom_k_minus_1 = mult / k;
    }

    /* Second part: 2^98 * binom(n+48, 97) */
    seq_t binom2 = 0;
    if (n + 48 >= 97) {
        idx_t n2 = n + 48;
        binom2 = 1;
        for (idx_t j = 1; j <= 97; ++j)
            binom2 = binom2 * (n2 - j + 1) / j;  /* compute binom(n+48, 97) */
    }
    sum += pow2k[98] * binom2;

    return sum;
}
