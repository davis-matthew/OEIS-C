/*
 * A100241  In binary representation of n, replace ones with n and zeros with
 *          n's binary complement (keeping leading zeros).
 * https://oeis.org/A100241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = For the binary representation of n (L bits), replace each
 *            1 with the binary representation of n and each 0 with the
 *            bit‑wise complement of n (within L bits). Concatenate these L
 *            blocks to obtain the binary representation of a(n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   bitlen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen.h"

seq_t a(idx_t n)
{
    /* L = number of bits in n (1 for n = 0) */
    idx_t L = bitlen(n);

    /* Convert n to the output type so we can shift and mask */
    seq_t sn = (seq_t)n;

    /* Create mask for L bits */
    idx_t bits = sizeof(seq_t) * 8;
    seq_t mask;
    if (L >= bits) {
        mask = (seq_t)-1;                  /* all bits set */
    } else {
        mask = ((seq_t)1 << L) - 1;         /* lower L bits set */
    }

    /* Complement of n within L bits */
    seq_t comp = (~sn) & mask;

    seq_t res = 0;

    /* Iterate over bits of n from most significant to least */
    for (idx_t i = 0; i < L; ++i) {
        bool bit = (n >> (L - 1 - i)) & 1;   /* bit i of n */
        seq_t group = bit ? sn : comp;
        /* Append group: shift left L bits and OR */
        res = (res << L) | group;
    }

    return res;
}
