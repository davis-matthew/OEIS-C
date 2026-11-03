/*
 * A133017  Decimal expansion of the constant formed by concatenating the
 *          imperfect numbers.
 * https://oeis.org/A133017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 94 terms through n = 888 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133017: Decimal expansion of the constant formed by concatenating
   all imperfect numbers (all positive integers except perfect numbers).  The
   function returns the n-th digit (0‑based) of that infinite concatenated
   string. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* List of all perfect numbers that fit in 64 bits.  Only the smallest
   few are relevant for n up to 10 000, but the table covers the full range
   of 64‑bit values.  The array is sorted increasingly. */
static bool is_perfect(uint64_t x)
{
    static const uint64_t perfect[] = {
        6ULL,
        28ULL,
        496ULL,
        8128ULL,
        33550336ULL,
        8589869056ULL,
        137438691328ULL,
        2305843008139952128ULL
    };
    size_t i;
    for (i = 0; i < sizeof(perfect) / sizeof(perfect[0]); ++i) {
        if (perfect[i] == x) return true;
        if (perfect[i] > x) break;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t idx = 0;      /* position in the concatenated digit stream   */
    uint64_t k = 1;     /* current integer to consider                */

    for (;;) {
        if (is_perfect(k)) {      /* skip perfect numbers */
            ++k;
            continue;
        }

        /* Extract digits of k into a temporary buffer, most significant
           digit first.  Since k ≤ 2^64−1, at most 20 decimal digits.  */
        uint8_t dig[20];
        int dlen = 0;
        uint64_t tmp = k;
        do {
            dig[dlen++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        } while (tmp);

        /* Emit digits from most significant to least.  The dlen‑1 index
           corresponds to the most significant digit.  */
        int i;
        for (i = dlen - 1; i >= 0; --i) {
            if (idx == n) {
                return (seq_t)dig[i];
            }
            ++idx;
        }
        ++k;
    }
}
