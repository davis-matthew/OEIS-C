/*
 * A005035  Number of nonequivalent dissections of a polygon into n
 *          quadrilaterals by nonintersecting diagonals rooted at a cell up to
 *          rotation and reflection.
 * https://oeis.org/A005035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. A(z) = ((G(z^2)-1)/z + z^2*G(z^2)^4 + z*(G(z)^4 + G(z^2)^2
 *            + 2*G(z^4))/4)/2, G(z) = 1 + z*G(z)^3. Uses [z^m] G(z)^k =
 *            k/(3m+k) * C(3m+k, m) =: g_k(m), so a(n) = (4*[n
 *            odd]*g_1((n+1)/2) + 4*[n even]*g_4((n-2)/2) + g_4(n-1) + [n
 *            odd]*g_2((n-1)/2) + 2*[n == 1 mod 4]*g_1((n-1)/4)) / 8
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 27, u128 n <= 50,
 *            big any n
 * Helpers:   b128, binom, g128, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/u128_to_seq.h"
#include "../utils/seq_is_big.h"
#include "../utils/b128.h"
#include "../utils/g128.h"

static seq_t gseq(idx_t k, idx_t m) {
    idx_t N = 3 * m + k;
    seq_t r = binom(N, m);
    if (m > 0) r -= 3 * binom(N - 1, m - 1);
    return r;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (!seq_is_big() && sizeof(seq_t) <= 8) {
        unsigned __int128 s = g128(4, n - 1);
        if (n % 2 == 1) {
            s += 4 * g128(1, (n + 1) / 2);
            s += g128(2, (n - 1) / 2);
        } else {
            s += 4 * g128(4, (n - 2) / 2);
        }
        if (n % 4 == 1) s += 2 * g128(1, (n - 1) / 4);
        return u128_to_seq(s / 8);
    } else {
        seq_t s = gseq(4, n - 1);
        if (n % 2 == 1) {
            s += 4 * gseq(1, (n + 1) / 2);
            s += gseq(2, (n - 1) / 2);
        } else {
            s += 4 * gseq(4, (n - 2) / 2);
        }
        if (n % 4 == 1) s += 2 * gseq(1, (n - 1) / 4);
        return s / 8;
    }
}
