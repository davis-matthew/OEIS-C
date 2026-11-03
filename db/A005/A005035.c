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
 * Validated: standard, pass, 40 terms through n = 199 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    
    seq_t result = 1;
    for (idx_t i = 1; i <= k; i++) {
        result = result * (seq_t)(n - i + 1) / (seq_t)i;
    }
    return result;
}

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

static unsigned __int128 b128(uint64_t N, uint64_t k) {
    unsigned __int128 r = 1;
    uint64_t i;
    for (i = 0; i < k; i++) r = r * (unsigned __int128)(N - i) / (i + 1);
    return r;
}

static unsigned __int128 g128(uint64_t k, uint64_t m) {
    uint64_t N = 3 * m + k;
    return b128(N, m) * k / N;
}

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
