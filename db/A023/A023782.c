/*
 * A023782  Metadromes: digits in base 14 are in strict ascending order.
 * https://oeis.org/A023782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   a(n) = nth metadrome of base‑14 digits that are strictly
 *            increasing (0 is a(1)). The sequence is finite: 8192 terms
 * Range:     u8 n <= 113, u16 n <= 1587, u32 n <= 7759, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Combinatorial helper: C(n,k) for 0 <= k <= n <= 13.  */
static inline idx_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
        r = r * (n - k + i) / i;
    return r;
}

/* Return the nth metadrome (index 1 = 0).  */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;          /* 0 is the first term */

    idx_t m = n - 1;                      /* offset among non‑zero terms */

    /* Find the length of the number (digits count).  */
    idx_t cumulative = 0;
    size_t k = 0;                         /* 0‑based index for length-1 */
    for (k = 0; k < 13; ++k) {            /* at most 13 digits */
        idx_t count = binom_idx(13, k + 1);
        if (m <= cumulative + count)
            break;
        cumulative += count;
    }
    if (k == 13)                          /* out of range: return 0 */
        return (seq_t)0;

    idx_t length = k + 1;                  /* number of digits */
    idx_t rank   = m - cumulative - 1;     /* 0‑based rank within this length */

    /* Decode the rank into the corresponding combination of digits 1..13. */
    idx_t digits[13];
    idx_t pos      = 0;
    idx_t start    = 1;
    idx_t remaining = length;
    idx_t rem_rank  = rank;

    while (pos < length) {
        idx_t max_digit = 13 - (remaining - 1);
        for (idx_t d = start; d <= max_digit; ++d) {
            idx_t comb = binom_idx(13 - d, remaining - 1);
            if (rem_rank < comb) {
                digits[pos++] = d;
                start = d + 1;
                --remaining;
                break;
            } else
                rem_rank -= comb;
        }
    }

    /* Convert the chosen digits into the actual number.  */
    seq_t val = 0;
    for (idx_t i = 0; i < length; ++i) {
        val = val * (seq_t)14 + (seq_t)digits[i];
    }

    return val;
}
