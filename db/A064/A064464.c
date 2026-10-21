/*
 * A064464  Binary order (cf. A029837) of the number of parts if 3^n is
 *          partitioned into parts of size 2^n as far as possible and into
 *          parts of size 1^n (cf. A060692).
 * https://oeis.org/A064464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(log_2(A060692(n))) where * A060692(n) = floor(3^n
 *            / 2^n) + (3^n mod 2^n)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   count_leading_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/count_leading_zeros.h"

static const size_t MAX_LIMBS = 300;   /* enough for n up to about 10 000 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but guard */

    /* big integer storage: little‑endian limbs */
    uint64_t big[MAX_LIMBS];
    size_t  big_len = 1;
    big[0] = 1;                            /* 3^0 = 1 */

    /* compute 3^n by repeated multiplication by 3 */
    for (idx_t i = 0; i < n; ++i) {
        unsigned __int128 carry = 0;
        for (size_t j = 0; j < big_len; ++j) {
            unsigned __int128 tmp = (unsigned __int128)big[j] * 3 + carry;
            big[j] = (uint64_t)tmp;
            carry  = tmp >> 64;
        }
        if (carry) {
            if (big_len >= MAX_LIMBS) return (seq_t)0; /* overflow guard */
            big[big_len++] = (uint64_t)carry;
        }
    }

    /* shift parameters */
    size_t shift_words = n >> 6;           /* n / 64 */
    uint64_t shift_bits = n & 63;          /* n % 64 */

    /* quotient q = floor(3^n / 2^n) */
    uint64_t q[MAX_LIMBS];
    size_t   qlen = 0;
    if (big_len > shift_words) {
        qlen = big_len - shift_words;
        if (shift_bits == 0) {
            memcpy(q, &big[shift_words], qlen * sizeof(uint64_t));
        } else {
            for (size_t i = 0; i < qlen; ++i) {
                uint64_t low  = (shift_words + i < big_len) ? big[shift_words + i] : 0;
                uint64_t high = (shift_words + i + 1 < big_len) ? big[shift_words + i + 1] : 0;
                q[i] = (low >> shift_bits) | (high << (64 - shift_bits));
            }
        }
        /* trim leading zeros */
        while (qlen > 0 && q[qlen - 1] == 0) --qlen;
    }

    /* remainder r = 3^n mod 2^n (lower n bits) */
    size_t rlen = (n + 63) >> 6;        /* ceil(n / 64) */
    uint64_t r[MAX_LIMBS];
    memset(r, 0, sizeof(r));
    for (size_t i = 0; i < rlen && i < big_len; ++i) r[i] = big[i];
    if (shift_bits != 0 && rlen > 0) {
        uint64_t mask = (1ULL << shift_bits) - 1;
        r[rlen - 1] &= mask;
    }
    /* trim trailing zeros */
    while (rlen > 0 && r[rlen - 1] == 0) --rlen;

    /* total = q + r */
    uint64_t total[MAX_LIMBS];
    size_t   tlen = (qlen > rlen ? qlen : rlen);
    unsigned __int128 carry = 0;
    size_t i;
    for (i = 0; i < tlen; ++i) {
        unsigned __int128 sum = carry;
        if (i < qlen) sum += q[i];
        if (i < rlen) sum += r[i];
        total[i] = (uint64_t)sum;
        carry    = sum >> 64;
    }
    if (carry) total[tlen++] = (uint64_t)carry;

    /* find msb index */
    size_t limb_idx = tlen - 1;
    uint64_t limbs_val = total[limb_idx];
    int leading_zeros = count_leading_zeros(limbs_val);
    int msb_in_limb = 63 - leading_zeros;
    unsigned msb_index = limb_idx * 64 + msb_in_limb;

    /* check power of two */
    int is_pow2 = 1;
    for (size_t j = 0; j < tlen; ++j) {
        if (j == limb_idx) {
            if ((total[j] & (total[j] - 1)) != 0) { is_pow2 = 0; break; }
        } else {
            if (total[j] != 0) { is_pow2 = 0; break; }
        }
    }

    seq_t result = is_pow2 ? (seq_t)msb_index : (seq_t)(msb_index + 1);
    return result;
}
