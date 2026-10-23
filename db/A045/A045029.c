/*
 * A045029  Numbers whose base-4 representation contains exactly three 0's and
 *          one 1.
 * https://oeis.org/A045029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th smallest integer whose base‑4 representation
 *            contains exactly three 0's and one 1 (leading digit non‑zero)
 * Range:     u8 n <= 1, u16 n <= 3577, u32 n <= 4300, u64 n <= 4300,
 *            u128 n <= 4300, big any n
 * Helpers:   add_cap, mul_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/add_cap.h"
#include "../utils/mul_cap.h"

/* Helper: compute binomial coefficient C(n,k), saturating at limit+1 */
static idx_t binom_cap(idx_t n, idx_t k, idx_t limit) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        unsigned __int128 num = n - k + i;
        unsigned __int128 den = i;
        res = res * num / den;
        if (res > (unsigned __int128)(limit + 1)) return limit + 1;
    }
    return (idx_t)res;
}

/* Count numbers of length L with the constraints, saturating at limit+1 */
static idx_t count_l_cap(idx_t L, idx_t limit) {
    if (L < 4) return 0;
    /* case A : 1 in the most significant position */
    idx_t binA = binom_cap(L - 1, 3, limit);
    unsigned __int128 powA128 = ((unsigned __int128)1) << (L - 4);
    idx_t powA = (powA128 > (unsigned __int128)(limit + 1)) ? (idx_t)(limit + 1) : (idx_t)powA128;
    idx_t partA = mul_cap(binA, powA, limit);
    /* case B : 1 not in the most significant position */
    idx_t binB = binom_cap(L - 2, 3, limit);
    idx_t partB = mul_cap(mul_cap(L - 1, binB, limit), powA, limit);
    return add_cap(partA, partB, limit);
}

/* Count completions for remaining positions, saturating at limit+1 */
static idx_t count_completions_cap(idx_t rem_pos, idx_t new_z, idx_t new_o, idx_t limit) {
    if (new_z > rem_pos || new_o > rem_pos - new_z) return 0;
    idx_t free = rem_pos - new_z - new_o;
    idx_t bin1 = binom_cap(rem_pos, new_z, limit);
    idx_t bin2 = binom_cap(rem_pos - new_z, new_o, limit);
    idx_t part = mul_cap(bin1, bin2, limit);
    /* multiply by 2^free */
    if (free > 0) {
        unsigned __int128 tmp = (unsigned __int128)part;
        tmp <<= free;
        if (tmp > (unsigned __int128)(limit + 1)) tmp = limit + 1;
        part = (idx_t)tmp;
    }
    return part;
}

seq_t a(idx_t n) {
    /* Find the length L for the n-th term */
    idx_t cumulative = 0;
    idx_t L = 4;
    for (;;) {
        idx_t need = n - cumulative;
        idx_t cntL = count_l_cap(L, need);
        if (need <= cntL) break;
        cumulative += cntL;
        ++L;
    }
    idx_t offset = n - cumulative;          /* 1‑based within length L */

    seq_t result = 0;
    idx_t o = 1, z = 3;                      /* remaining ones and zeros */

    for (idx_t pos = 0; pos < L; ++pos) {
        for (int d = 0; d < 4; ++d) {
            if (pos == 0 && d == 0) continue;          /* no leading zero */
            if (d == 0 && z == 0) continue;           /* no zeros left */
            if (d == 1 && o == 0) continue;           /* no ones left */

            idx_t rem_pos   = L - pos - 1;
            idx_t next_z = z - (d == 0 ? 1 : 0);
            idx_t next_o = o - (d == 1 ? 1 : 0);

            idx_t cnt = count_completions_cap(rem_pos, next_z, next_o, offset);
            if (offset > cnt) {
                offset -= cnt;                         /* skip these */
            } else {
                result = result * (seq_t)4 + (seq_t)d;  /* choose this digit */
                z = next_z;
                o = next_o;
                break;
            }
        }
    }
    return result;
}
