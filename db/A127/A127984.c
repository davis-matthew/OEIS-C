/*
 * A127984  a(n) = (n/3 + 7/9)*2^(n - 1) + (-1)^n/9.
 * https://oeis.org/A127984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((3*n + 7)*2^(n-1) + (-1)^n)/9
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (derived from a(n) = (n/3 + 7/9)*2^(n-1) + (-1)^n/9) */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Ensure n >= 1 (offset 1) */
    seq_t three_n_plus7 = ((seq_t)3 * n) + (seq_t)7;
    seq_t pow2_val = pow2(n - 1);     /* 2^(n-1) */

#ifdef __SIZEOF_INT128__
    /* Use 128‑bit arithmetic when seq_t is 64‑bit or smaller */
    if (sizeof(seq_t) <= 8) {
        __int128_t tmp = (__int128_t)three_n_plus7 * (__int128_t)pow2_val;
        tmp += (n & 1U) ? -1 : 1;  /* add 1 if n even, subtract 1 if n odd */
        tmp /= 9;
        return (seq_t)tmp;
    }
#endif

    /* Fallback for larger seq_t or when 128‑bit is unavailable */
    seq_t tmp = three_n_plus7 * pow2_val;
    if (n & 1U) {          /* n odd */
        tmp -= (seq_t)1;
    } else {               /* n even */
        tmp += (seq_t)1;
    }
    return tmp / 9;
}
