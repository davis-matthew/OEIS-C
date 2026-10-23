/*
 * A115932  Numbers k such that k and 8*k, taken together, are zeroless
 *          pandigital.
 * https://oeis.org/A115932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers k such that k and 8*k together use all
 * digits 1–9 exactly once (no zeros, no repeated digits).
 * The sequence contains 46 terms; the largest is 9541.
 * The implementation precomputes all terms on first call and
 * returns the n‑th one (1‑based).  For n beyond the length
 * it returns 0.  This works for any seq_t large enough to hold
 * the terms; terms that do not fit in seq_t simply wrap when
 * cast to seq_t (tests for such small types ignore those terms). */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>   /* for size_t */

#define MAX_TERMS 46
static const idx_t LOWER = 1250;   /* smallest k with 5‑digit 8*k */
static const idx_t UPPER = 10000;  /* exclusive upper bound */

static uint64_t cache64[MAX_TERMS];   /* precomputed terms */
static size_t cache_cnt = 0;
static bool cache_built = false;

seq_t a(idx_t n) {
    /* Build cache on first call */
    if (!cache_built) {
        for (idx_t i = LOWER; i < UPPER && cache_cnt < MAX_TERMS; ++i) {
            uint64_t k = (uint64_t)i;
            uint64_t m = k * 8ULL;

            uint32_t mask = 0;   /* bitmask for digits 0–9 */
            bool ok = true;

            /* process digits of k */
            uint64_t tmp = k;
            while (tmp > 0 && ok) {
                uint32_t digit = (uint32_t)(tmp % 10ULL);
                if (digit == 0 || (mask & (1U << digit))) {
                    ok = false;
                    break;
                }
                mask |= (1U << digit);
                tmp /= 10ULL;
            }

            /* process digits of 8*k */
            tmp = m;
            while (tmp > 0 && ok) {
                uint32_t digit = (uint32_t)(tmp % 10ULL);
                if (digit == 0 || (mask & (1U << digit))) {
                    ok = false;
                    break;
                }
                mask |= (1U << digit);
                tmp /= 10ULL;
            }

            /* exactly digits 1–9 must be used */
            if (ok && mask == 0x3FEU) {
                cache64[cache_cnt++] = k;
            }
        }
        cache_built = true;
    }

    if (n == 0 || n > cache_cnt)
        return (seq_t)0;

    return (seq_t)cache64[(size_t)(n - 1)];
}
