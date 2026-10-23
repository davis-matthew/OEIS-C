/*
 * A169885  Cubes (n * n * n) in carryless arithmetic mod 10.
 * https://oeis.org/A169885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) is the cube of n in carryless base‑10 arithmetic * (digits
 *            are multiplied modulo 10, with no carry)
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 1999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Convert n into decimal digits (least significant first). */
    unsigned char nd[21];           /* 2^64‑1 has at most 20 decimal digits. */
    size_t len_n = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        nd[0] = 0;
        len_n = 1;
    } else {
        while (tmp > 0) {
            nd[len_n++] = tmp % 10;
            tmp /= 10;
        }
    }

    /* Compute n² in carryless arithmetic (digits modulo 10). */
    size_t len_sq = len_n * 2 - 1;
    unsigned char sq[41];           /* 2·20−1 = 39, 41 for safety. */
    for (size_t i = 0; i < len_sq; ++i) sq[i] = 0;
    for (size_t i = 0; i < len_n; ++i) {
        for (size_t j = 0; j < len_n; ++j) {
            int prod = nd[i] * nd[j] + sq[i + j];
            sq[i + j] = prod % 10;
        }
    }

    /* Compute n³ = n² · n in carryless arithmetic. */
    size_t len_cu = len_sq + len_n - 1;   /* 3·len_n − 2 */
    unsigned char cu[61];                 /* 3·20−2 = 58, 61 for safety. */
    for (size_t i = 0; i < len_cu; ++i) cu[i] = 0;
    for (size_t i = 0; i < len_sq; ++i) {
        for (size_t j = 0; j < len_n; ++j) {
            int prod = sq[i] * nd[j] + cu[i + j];
            cu[i + j] = prod % 10;
        }
    }

    /* Assemble the result into a seq_t value. */
    seq_t val = 0;
    for (size_t i = len_cu; i > 0; --i) {
        val = val * 10 + cu[i - 1];
    }
    return val;
}
