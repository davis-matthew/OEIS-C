/*
 * A212452  Ceiling(3n + log(3n)).
 * https://oeis.org/A212452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(3n + log(3n)) (natural log)
 * Range:     u8 n <= 83, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0) is outside the sequence offset; return 0 for safety */
    if (n == 0) return 0;

    /* Pre‑computed ceil(e^k) for k = 0 … 12 (covers 3n up to 30000) */
    static const idx_t ceil_e_pow[] = {
        1ULL,    /* ceil(e^0)  = 1  */
        3ULL,    /* ceil(e^1)  = 3  */
        8ULL,    /* ceil(e^2)  = 8  */
        21ULL,   /* ceil(e^3)  = 21 */
        55ULL,   /* ceil(e^4)  = 55 */
        149ULL,  /* ceil(e^5)  = 149 */
        404ULL,  /* ceil(e^6)  = 404 */
        1097ULL, /* ceil(e^7)  = 1097 */
        2981ULL, /* ceil(e^8)  = 2981 */
        8104ULL, /* ceil(e^9)  = 8104 */
        22027ULL,/* ceil(e^10) = 22027 */
        59875ULL /* ceil(e^11) = 59875 */
    };
    const size_t thresh_cnt = sizeof(ceil_e_pow) / sizeof(ceil_e_pow[0]);

    /* v = 3n  (guaranteed to fit in idx_t for the problem limits) */
    idx_t v = n * 3ULL;

    /* Find largest k with ceil(e^k) ≤ v. */
    idx_t k = 0;
    while (k + 1 < thresh_cnt && ceil_e_pow[k + 1] <= v)
        ++k;

    /* a(n) = 3n + 1 + floor(log(3n)) = 3n + (k + 1) */
    return ((seq_t)n) * 3ULL + ((seq_t)(k + 1));
}
