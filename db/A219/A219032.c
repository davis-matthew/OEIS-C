/*
 * A219032  Number of distinct squares as subwords of decimal representation
 *          of n-th square.
 * https://oeis.org/A219032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct perfect–square substrings of the
 *            decimal representation of n^2 (OEIS A219032)
 * Range:     u8 n <= 15, u16 n <= 845, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits, is_square_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/get_digits.h"
#include "../utils/is_square_ull.h"

seq_t a(idx_t n)
{
    /* Compute n².  Use seq_t arithmetic to support arbitrary-precision. */
    seq_t n_seq = (seq_t)n;
    seq_t sq = n_seq * n_seq;

    /* Extract decimal digits of sq.  get_digits returns least‑significant
       digit first (little endian). */
    uint8_t digits[128];
    size_t dlen = 0;
    get_digits(sq, digits, &dlen);

    /* Store distinct squares found as unsigned long long.  Max possible
       substrings for dlen <= 9 is 45, so a small fixed array suffices. */
    unsigned long long seen[64];
    size_t seen_cnt = 0;

    /* Iterate over all substrings, starting from most significant digit
       (index dlen-1) down to least significant (0). */
    for (size_t i = dlen; i > 0; --i) {
        unsigned long long val = 0;
        for (size_t j = i; j > 0; --j) {
            val = val * 10ULL + (unsigned long long)digits[j - 1];
            if (!is_square_ull(val)) continue;
            /* duplicate detection */
            bool already = false;
            for (size_t k = 0; k < seen_cnt; ++k) {
                if (seen[k] == val) { already = true; break; }
            }
            if (!already) {
                if (seen_cnt < sizeof(seen) / sizeof(seen[0])) {
                    seen[seen_cnt++] = val;
                }
            }
        }
    }
    return (seq_t)seen_cnt;
}
