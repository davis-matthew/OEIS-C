/*
 * A046150  Largest n-digit number with maximal multiplicative persistence
 *          A014553.
 * https://oeis.org/A046150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = if n < 17: use the known optimal patterns if n >= 17:
 *            a(n) = 99999999998777772 * 10^(n-17) + (10^(n-17)-1)/9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   pow10, seq_from_str, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_from_str.h"
#include "../utils/pow10.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Handle the first 16 values by explicit pattern construction. */
    switch (n) {
        case 1:  return seq_from_str("9");
        case 2:  return seq_from_str("77");
        case 3:  return seq_from_str("976");
        case 4:  return seq_from_str("8876");
        case 5:  return seq_from_str("98886");
        case 6:  return seq_from_str("997762");
        case 7:  return seq_from_str("9999996");
        case 8:  return seq_from_str("99988862");
        case 9:  return seq_from_str("999888621");
        case 10: return seq_from_str("9998888773");
        case 11: return seq_from_str("99988887731");
        case 12: return seq_from_str("999888877311");
        case 13: return seq_from_str("9998888773111");
        case 14: return seq_from_str("99988887731111");
        case 15: return seq_from_str("998888887777772");
        case 16: return seq_from_str("9988888877777721");
        default: break;
    }

    /* For n >= 17 we use the compact formula. */
    idx_t k = n - 17;                     /* exponent of 10 */

    /* Seed constant 99999999998777772 */
    static seq_t seed = 0;
    static bool seed_set = false;
    if (!seed_set) {
        seed = seq_from_str("99999999998777772");
        seed_set = true;
    }

    seq_t pow10k = pow10(k);              /* 10^k */
    seq_t part1  = seq_mul(seed, pow10k); /* seed * 10^k */

    if (k == 0)                           /* 10^(n-17) = 1 */
        return part1;

    /* Build (10^k - 1)/9 : a repunit with k ones. */
    seq_t repunit = 0;
    for (idx_t i = 0; i < k; ++i) {
        repunit = seq_mul(repunit, 10);
        repunit = repunit + 1;
    }

    return part1 + repunit;
}
