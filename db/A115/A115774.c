/*
 * A115774  Integers i such that 15*i = A048720bi(23,i).
 * https://oeis.org/A115774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer i (0‑based) such that * 15 * i equals
 *            the carryless (GF(2)[X]) product of 23 and i. * * The carryless
 *            product is computed as the XOR of 23 shifted left by * every set
 *            bit position of i. That is: * carryless_mul(23,i) = ⊕_{j : bit j
 *            of i = 1} (23 << j) * * This implementat...
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Precomputed 23 << j for j = 0 .. 63 (used for speed). */
static const seq_t _shifted_23[64] = {
    (seq_t)23ULL << 0U,
    (seq_t)23ULL << 1U,
    (seq_t)23ULL << 2U,
    (seq_t)23ULL << 3U,
    (seq_t)23ULL << 4U,
    (seq_t)23ULL << 5U,
    (seq_t)23ULL << 6U,
    (seq_t)23ULL << 7U,
    (seq_t)23ULL << 8U,
    (seq_t)23ULL << 9U,
    (seq_t)23ULL << 10U,
    (seq_t)23ULL << 11U,
    (seq_t)23ULL << 12U,
    (seq_t)23ULL << 13U,
    (seq_t)23ULL << 14U,
    (seq_t)23ULL << 15U,
    (seq_t)23ULL << 16U,
    (seq_t)23ULL << 17U,
    (seq_t)23ULL << 18U,
    (seq_t)23ULL << 19U,
    (seq_t)23ULL << 20U,
    (seq_t)23ULL << 21U,
    (seq_t)23ULL << 22U,
    (seq_t)23ULL << 23U,
    (seq_t)23ULL << 24U,
    (seq_t)23ULL << 25U,
    (seq_t)23ULL << 26U,
    (seq_t)23ULL << 27U,
    (seq_t)23ULL << 28U,
    (seq_t)23ULL << 29U,
    (seq_t)23ULL << 30U,
    (seq_t)23ULL << 31U,
    (seq_t)23ULL << 32U,
    (seq_t)23ULL << 33U,
    (seq_t)23ULL << 34U,
    (seq_t)23ULL << 35U,
    (seq_t)23ULL << 36U,
    (seq_t)23ULL << 37U,
    (seq_t)23ULL << 38U,
    (seq_t)23ULL << 39U,
    (seq_t)23ULL << 40U,
    (seq_t)23ULL << 41U,
    (seq_t)23ULL << 42U,
    (seq_t)23ULL << 43U,
    (seq_t)23ULL << 44U,
    (seq_t)23ULL << 45U,
    (seq_t)23ULL << 46U,
    (seq_t)23ULL << 47U,
    (seq_t)23ULL << 48U,
    (seq_t)23ULL << 49U,
    (seq_t)23ULL << 50U,
    (seq_t)23ULL << 51U,
    (seq_t)23ULL << 52U,
    (seq_t)23ULL << 53U,
    (seq_t)23ULL << 54U,
    (seq_t)23ULL << 55U,
    (seq_t)23ULL << 56U,
    (seq_t)23ULL << 57U,
    (seq_t)23ULL << 58U,
    (seq_t)23ULL << 59U,
    (seq_t)23ULL << 60U,
    (seq_t)23ULL << 61U,
    (seq_t)23ULL << 62U,
    (seq_t)23ULL << 63U
};

/* Check whether integer i satisfies 15*i == carryless(23,i). */
static bool valid_uint64(idx_t i)
{
    /* Compute carryless product of 23 and i. */
    seq_t carryless = 0;
    idx_t tmp = i;
    while (tmp != 0) {
        /* Position of lowest set bit. */
        int pos = (int)__builtin_ctzll(tmp);
        carryless ^= _shifted_23[pos];
        tmp &= tmp - 1;          /* remove that bit */
    }

    /* Compute ordinary product. */
    seq_t prod = (seq_t)15 * (seq_t)i;

    return prod == carryless;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t found = 0;
    for (idx_t i = 1; ; ++i) {
        if (valid_uint64(i)) {
            ++found;
            if (found == n) {
                return (seq_t)i;
            }
        }
    }
}
