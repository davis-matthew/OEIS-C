/*
 * A033032  Numbers all of whose base 6 digits are odd.
 * https://oeis.org/A033032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th number whose base‑6 representation uses only
 *            the digits 1,3,5. For n≥1, find the block of numbers with the
 *            same number of base‑6 digits. Let L be the smallest integer such
 *            that (3^L–1)/2 ≤ n. Compute the offset within this block, write
 *            this offset in base‑3 with exactly L digits,...
 * Range:     u8 n <= 39, u16 n <= 1335, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are provided by the build.           */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Undefined for n == 0, but return 0 safely. */
    if (n == 0) return 0;

    /* Find the largest power of 3, pow3 = 3^L,
       such that (pow3 – 1)/2 ≤ n. */
    idx_t pow3 = 1;          /* 3^0 */
    int L = 0;               /* current digit count */

    while (1) {
        if (pow3 > (UINT64_MAX / 3)) break;           /* avoid overflow   */
        idx_t next_pow3 = pow3 * 3;                   /* 3^(L+1)          */
        idx_t threshold = (next_pow3 - 1) / 2;        /* start of next block */
        if (threshold <= n) {
            pow3 = next_pow3;
            L++;
        } else {
            break;
        }
    }

    /* Index offset inside this block (0 ≤ off < 3^L). */
    idx_t block_start = (pow3 - 1) / 2;
    idx_t off = n - block_start;

    /* Convert offset to base‑3 digits (least significant first). */
    uint8_t d[64] = {0};            /* 64 > max possible L for 64‑bit n */
    idx_t tmp = off;
    for (int i = 0; i < L; ++i) {
        d[i] = tmp % 3;
        tmp /= 3;
    }

    /* Build the result from most significant digit. */
    seq_t res = 0;
    for (int i = L - 1; i >= 0; --i) {
        res = res * 6 + (seq_t)(1 + 2 * d[i]); /* map 0→1, 1→3, 2→5 */
    }

    return res;
}
