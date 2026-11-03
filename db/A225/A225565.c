/*
 * A225565  Numbers with repeated digits, whose squares do not contain any
 *          repeated digits.
 * https://oeis.org/A225565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the nth integer that has at least one repeated decimal
 *            digit and whose square’s decimal representation contains all
 *            distinct digits. The function computes terms by brute‑force
 *            search; no pre‑computed table is used
 * Range:     u8 n <= 20, u16 n <= 243, u32 n <= 274, u64 n <= 274,
 *            u128 n <= 274, big any n
 * Validated: full, pass, 274 terms through n = 274 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Return true if the unsigned 64‑bit integer n contains no repeated digit. */
static bool has_no_repeated_digits_uint64(uint64_t n)
{
    bool seen[10] = {0};
    while (n != 0) {
        int d = n % 10;
        if (seen[d])
            return false;
        seen[d] = true;
        n /= 10;
    }
    return true;
}

/* Return true if the unsigned 64‑bit integer n contains a repeated digit. */
static bool has_repeated_digits_uint64(uint64_t n)
{
    bool seen[10] = {0};
    while (n != 0) {
        int d = n % 10;
        if (seen[d])
            return true;
        seen[d] = true;
        n /= 10;
    }
    return false;
}

/* ----------  OEIS sequence A225565  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;             /* only defined for n >= 1 */

    idx_t count = 0;
    uint64_t cand = 33;               /* a(1) = 33 */
    const uint64_t UPPER = 1000000;   /* suffices for all 274 terms */

    while (cand <= UPPER) {
        if (has_repeated_digits_uint64(cand)) {
            uint64_t sq = cand * cand;   /* works for all 64‑bit values */
            if (has_no_repeated_digits_uint64(sq)) {
                ++count;
                if (count == n)
                    return (seq_t)cand;
            }
        }
        ++cand;
    }

    return (seq_t)0;   /* n is out of range (n > 274) */
}
