/*
 * A035801  Coordination sequence for lattice D*_32 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{min(32,n)} 2^k * C(32,k) * C(n-1,k-1) + 2^32 *
 *            C(n+15,31) (with the convention C(-1,-1)=1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t etc. */
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Helper to pre‑compute the constants C(32,k) */
static seq_t C32[33] = {0};
static int C32_init = 0;

static void init_C32(void) {
    if (!C32_init) {
        for (idx_t k = 0; k <= 32; ++k)
            C32[k] = binomial(32, k);
        C32_init = 1;
    }
}

seq_t a(idx_t n)
{
    /* Ensure the lookup table for C(32,k) is ready. */
    init_C32();

    /* Handle the special first term a(0) = 1 (C(-1,-1)=1). */
    if (n == 0)
        return (seq_t)1;

    seq_t total = 0;

    /* Sum over k = 1 .. min(32, n) */
    idx_t maxk = 32;
    if (n < maxk)
        maxk = n;
    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t bin32   = C32[k];
        seq_t binM1   = binomial(n - 1, k - 1);  /* C(n-1,k-1) */
        seq_t pow2k   = pow2(k);                 /* 2^k */

        /* Use 128‑bit intermediate to avoid overflow when seq_t is 64‑bit. */
        unsigned __int128 prod = (unsigned __int128)pow2k *
                                 (unsigned __int128)bin32 *
                                 (unsigned __int128)binM1;
        total += (seq_t)prod;
    }

    /* Add the second term if it contributes */
    if (n >= 16) {
        seq_t bin_part = binomial(n + 15, 31); /* C(n+15,31) */
        seq_t pow2_32  = pow2(32);             /* 2^32 */

        unsigned __int128 prod2 = (unsigned __int128)pow2_32 *
                                  (unsigned __int128)bin_part;
        total += (seq_t)prod2;
    }

    return total;
}
