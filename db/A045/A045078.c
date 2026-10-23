/*
 * A045078  Numbers whose base-4 representation contains exactly three 0's and
 *          two 3's.
 * https://oeis.org/A045078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-4 representation contains exactly three 0's and two 3's. */
/* This function enumerates base‑4 numbers in increasing order, checks the
 * required digit pattern, and returns the n‑th such number (1‑based index).  */
#include <stdint.h>
#include <stdbool.h>

/* Helper: test whether `val` of length `len` has exactly three 0's and two 3's. */
static inline bool matches_pattern(seq_t val, int len)
{
    int zeros = 0, threes = 0;
    seq_t t = val;
    for (int i = 0; i < len; ++i) {
        /* Retrieve the lowest 2 bits: value modulo 4. */
        uint8_t d = (uint8_t)(t & 3U);
        if (d == 0) {
            ++zeros;
            if (zeros > 3) return false;
        } else if (d == 3) {
            ++threes;
            if (threes > 2) return false;
        }
        t >>= 2;
    }
    return zeros == 3 && threes == 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for 0, safe guard */

    idx_t remaining = n;   /* number of matches still needed */

    /* Pre‑compute powers of 4 up to a safe bound. */
    const int MAX_L = 20;  /* sufficient for n ≤ 10 000 */
    seq_t pow4[MAX_L + 1];
    pow4[0] = (seq_t)1;
    for (int i = 1; i <= MAX_L; ++i)
        pow4[i] = pow4[i - 1] << 2;   /* * 4 */

    for (int len = 5; ; ++len) {
        if (len > MAX_L) break;  /* safeguard, should never happen for given n */

        seq_t start = pow4[len - 1];     /* smallest len‑digit number (1 followed by zeros) */
        seq_t end   = pow4[len] - 1;     /* largest len‑digit number (3…3) */

        for (seq_t val = start; val <= end; ++val) {
            if (matches_pattern(val, len)) {
                if (--remaining == 0)
                    return val;
            }
        }
    }

    /* The loop always returns for valid n. */
    return 0;
}
