/*
 * A044975  Numbers whose base-3 representation contains exactly two 0's and
 *          no 1's.
 * https://oeis.org/A044975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑3 representation contains exactly two 0’s and
 *            no 1’s
 * Range:     u8 n <= 10, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute powers of 3 up to 64 once */
    static seq_t pow3[65];
    static bool initialized = false;
    if (!initialized) {
        pow3[0] = (seq_t)1;
        for (int i = 1; i < 65; ++i)
            pow3[i] = pow3[i-1] * (seq_t)3;
        initialized = true;
    }

    /* Find the base‑3 length L that contains the nth number */
    idx_t cumulative = 0;
    idx_t L = 3;
    while (1) {
        idx_t count = (L - 1) * (L - 2) / 2;   /* choose(L‑1,2) */
        if (cumulative + count >= n)
            break;
        cumulative += count;
        ++L;
    }

    /* Rank of the desired number among numbers of this length (1‑based) */
    idx_t k = n - cumulative;

    /* Locate the two zero positions */
    idx_t remaining = k;
    idx_t z1 = 0, z2 = 0;
    for (int i = (int)(L - 2); i >= 1; --i) {
        idx_t block = (idx_t)i;
        if (remaining > block) {
            remaining -= block;
        } else {
            z1 = (idx_t)i;
            z2 = (idx_t)(i - remaining);
            break;
        }
    }

    /* Compute the value: 3^L − 1 − 2·3^z1 − 2·3^z2 */
    seq_t val = pow3[L];
    val -= (seq_t)1;
    seq_t sub = (seq_t)2 * pow3[z1] + (seq_t)2 * pow3[z2];
    val -= sub;
    return val;
}
