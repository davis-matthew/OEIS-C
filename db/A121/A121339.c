/*
 * A121339  Periodic part of continued fraction for square roots of integers.
 * https://oeis.org/A121339
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nice, nonn, tabf
 * Formula:   a(n) = the nth entry obtained by concatenating the periodic *
 *            parts of the continued‑fraction expansions of √k for all k≥2
 *            (non‑square * k only). The table is the list of partial
 *            quotients of the period of * √k (for square k the row is empty).
 *            The offset is 2: a(2) is the first * term of the firs...
 * Range:     u8 n <= 9101, u16 n <= 9101, u32 n <= 9101, u64 n <= 9101,
 *            u128 n <= 9101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Compute the floor of the square root of an unsigned 64‑bit integer. */
static inline uint64_t floor_sqrt_u64(uint64_t n) {
    uint64_t low = 0, high = ((uint64_t)1 << 32);  /* (2^32)^2 = 2^64 */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        uint64_t sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Compute the nth element of the flattened periodic parts of the
 * continued‑fraction expansions of √k.  n≥2. */
seq_t a(idx_t n) {
    /* next_index is the index of the first element in the current row.
     * For row k its first index is sum_{i<k}len(i)+2.  Start with 2. */
    idx_t next_index = 2;

    for (idx_t k = 2; ; ++k) {
        uint64_t kk = (uint64_t)k;               /* use 64‑bit arithmetic */
        uint64_t a0 = floor_sqrt_u64(kk);
        if (a0 * a0 == kk) {                     /* perfect square → empty row */
            continue;
        }

        /* Generate the periodic part of √k and store it in an array. */
        seq_t period[2048];
        idx_t plen = 0;

        uint64_t m = 0, d = 1, a = a0;
        do {
            m = (unsigned __int128)d * a - m;                       /* m = d*a - m */
            d = (uint64_t)(( (unsigned __int128)kk - (unsigned __int128)m * m ) / d); /* d = (k-m^2)/d */
            a = (a0 + m) / d;                                      /* a = ⌊(a0+m)/d⌋ */
            period[plen++] = (seq_t)a;                             /* store a */
        } while (a != 2 * a0 && plen < 2048);                      /* 2*a0 is the last term of the period */

        /* Check if the desired index falls inside this row. */
        if (next_index <= n && n < next_index + plen) {
            idx_t offset = n - next_index;
            return period[offset];
        }
        next_index += plen;   /* skip this row */
    }
}
