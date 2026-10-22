/*
 * A005513  Number of n-bead bracelets (turnover necklaces) of two colors with
 *          6 red beads and n-6 black beads.
 * https://oeis.org/A005513
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (24*C(n-1,5) + 3*(n+1)*(n-2)*(n-4) + 16*n)/288 if n == 0
 *            (mod 6); a(n) = (24*C(n-1,5) + 3*(n-1)*(n-3)*(n-5) + 16*n -
 *            48)/288 if n == 3 (mod 6); a(n) = (8*C(n-1,5) +
 *            (n+1)*(n-2)*(n-4))/96 if n == 2,4 (mod 6); a(n) = (8*C(n-1,5) +
 *            (n-1)*(n-3)*(n-5))/96 if n == 1,5 (mod 6)
 * Range:     u8 n <= 15, u16 n <= 42, u32 n <= 364, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   a005513_eval_seq, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"
#include "../utils/a005513_eval_seq.h"

typedef unsigned __int128 a005513_u128;

static a005513_u128 a005513_eval_u128(idx_t n) {
    a005513_u128 nn = (a005513_u128)n;
    a005513_u128 m = nn - 1;
    a005513_u128 c = 1;
    unsigned i;
    idx_t r = n % 6;
    for (i = 1; i <= 5; i++)
        c = c * (m - 5 + i) / i;
    if (r == 0)
        return (24 * c + 3 * (nn + 1) * (nn - 2) * (nn - 4) + 16 * nn) / 288;
    if (r == 3)
        return (24 * c + 3 * (nn - 1) * (nn - 3) * (nn - 5) + 16 * nn - 48) / 288;
    if (r == 2 || r == 4)
        return (8 * c + (nn + 1) * (nn - 2) * (nn - 4)) / 96;
    return (8 * c + (nn - 1) * (nn - 3) * (nn - 5)) / 96;
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width())
        return u128_to_seq(a005513_eval_u128(n));
    return a005513_eval_seq(n);
}
