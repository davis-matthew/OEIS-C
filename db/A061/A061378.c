/*
 * A061378  Product of all numbers formed by permuting the digits of n.
 * https://oeis.org/A061378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Πσ (Σi=0^{d-1} digit[i] * 10^{d-1-i}) where σ runs over
 *            all permutations of the positions of the decimal digits of n
 *            (each permutation counted separately, even if it yields the same
 *            number)
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   next_permutation_int, swap_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_permutation_int.h"
#include "../utils/swap_int.h"

seq_t a(idx_t n)
{
    /* Extract decimal digits of n (most significant first) */
    int digits[21];
    int rev[21];
    int d = 0;

    idx_t tmp = n;
    if (tmp == 0) {
        digits[0] = 0;
        d = 1;
    } else {
        while (tmp > 0) {
            rev[d++] = tmp % 10;
            tmp /= 10;
        }
        for (int i = 0; i < d; i++) digits[i] = rev[d - 1 - i];
    }

    /* Pre‑compute powers of 10: pow10[k] = 10^k   (k = 0 .. d-1)          */
    seq_t pow10[21];
    pow10[0] = (seq_t)1;
    for (int k = 1; k < d; ++k) pow10[k] = pow10[k - 1] * (seq_t)10;

    /* Permutation indices: 0 .. d-1 */
    int perm[21];
    for (int i = 0; i < d; ++i) perm[i] = i;

    seq_t product = (seq_t)1;

    do {
        seq_t value = (seq_t)0;
        for (int i = 0; i < d; ++i) {
            int dig = digits[perm[i]];
            value += (seq_t)dig * pow10[d - 1 - i];
        }
        product *= value;
    } while (next_permutation_int(perm, d));

    return product;
}
