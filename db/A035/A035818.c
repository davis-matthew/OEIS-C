/*
 * A035818  Coordination sequence for lattice D*_66 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..66} 2^k * binom(66,k) * binom(n-1,k-1) + 2^66 *
 *            binom((66+2n)/2-1, 65); a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

static seq_t pow2_arr[67];
static seq_t binom_n_arr[67];
static bool init = false;

static void init_arrays(void)
{
    if (init) return;
    pow2_arr[0] = (seq_t)1;
    for (idx_t i = 1; i <= (idx_t)66; ++i)
        pow2_arr[i] = pow2_arr[i-1] * (seq_t)2;
    for (idx_t k = 0; k <= (idx_t)66; ++k)
        binom_n_arr[k] = binomial((idx_t)66, k);
    init = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    init_arrays();

    seq_t res = (seq_t)0;
    for (idx_t k = 1; k <= (idx_t)66; ++k) {
        seq_t term = pow2_arr[k] * binom_n_arr[k];
        term = term * binomial(n - 1, k - 1);
        res += term;
    }

    idx_t sum = (idx_t)66 + ((idx_t)2 * n);
    idx_t arg = (sum >> 1) - (idx_t)1;
    res += pow2_arr[66] * binomial(arg, 65);

    return res;
}
