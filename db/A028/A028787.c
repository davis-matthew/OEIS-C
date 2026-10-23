/*
 * A028787  Nonsquares mod 74.
 * https://oeis.org/A028787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028787:  Nonsquares modulo 74
   a(n) = nth positive integer that is NOT a quadratic residue
   modulo 74 (i.e., its remainder modulo 37 is not a quadratic residue). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Quadratic residues modulo 37 (including 0).  Each residue
       corresponds to a quadratic residue modulo 74. */
    static const unsigned long long qr_mask =
        (1ULL << 0) | (1ULL << 1) | (1ULL << 3) |
        (1ULL << 4) | (1ULL << 7) | (1ULL << 9) |
        (1ULL << 10)| (1ULL << 11)| (1ULL << 12)|
        (1ULL << 16)| (1ULL << 21)| (1ULL << 25)|
        (1ULL << 26)| (1ULL << 27)| (1ULL << 28)|
        (1ULL << 30)| (1ULL << 33)| (1ULL << 34)|
        (1ULL << 36);
    /* Each block of 37 consecutive integers contains 18 nonsquares. */
    idx_t n0    = n - 1;                /* zero‑based index */
    idx_t blocks = n0 / 18;             /* number of full 37‑blocks passed */
    idx_t rem    = n0 % 18 + 1;         /* position within the current block */
    idx_t i      = blocks * 37 + 1;     /* first integer in the current block */

    idx_t cnt = 0;
    while (cnt < rem) {
        idx_t r = i % 37;
        if (((qr_mask >> r) & 1ULL) == 0) { /* not a quadratic residue */
            ++cnt;
            if (cnt == rem)
                return (seq_t)i;
        }
        ++i;
    }
    /* Unreachable: the loop always returns */
    return (seq_t)i;
}
