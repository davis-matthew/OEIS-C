/*
 * A072073  Number of solutions to cototient(x) = A051953(x) = 2^n.
 * https://oeis.org/A072073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + number of Mersenne‑prime exponents ≤ n (The solutions
 *            to m – φ(m) = 2ⁿ are m = 2ⁿ and m = 2ⁿ⁻ᵏ⁺¹ · (2ᵏ – 1) for each
 *            Mersenne prime exponent k ≤ n)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Known Mersenne‑prime exponents ≤ 10000. */
static const idx_t mersenne_exponents[] = {
    2, 3, 5, 7, 13, 17, 19, 31, 61, 89,
    107, 127, 521, 607, 1279, 2203, 2281,
    3217, 4253, 4423, 9689
};

static const size_t mersenne_count =
    sizeof(mersenne_exponents) / sizeof(mersenne_exponents[0]);

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (size_t i = 0; i < mersenne_count; ++i) {
        if (mersenne_exponents[i] <= n) {
            ++count;
        } else {
            break; /* the array is sorted */
        }
    }
    return (seq_t)(count + 1);
}
