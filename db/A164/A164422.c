/*
 * A164422  Number of binary strings of length n with no substrings equal to
 *          0000 0010 or 1010.
 * https://oeis.org/A164422
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4) + a(n-5) * (valid for n ≥ 9 with
 *            initial values * a(4)=13, a(5)=21, a(6)=35, a(7)=60, a(8)=102)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 84, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Count binary strings of length len that avoid the forbidden
 * 4‑bit substrings 0000 (0), 0010 (2) or 1010 (10).  len ≤ 8. */
static seq_t count_valid(uint64_t len)
{
    seq_t cnt = (seq_t)0;
    if (len > 64u) return cnt;                /* not needed for this seq */
    uint64_t limit = (uint64_t)1 << len;      /* 2^len strings */
    for (uint64_t mask = 0; mask < limit; ++mask) {
        bool bad = false;
        /* check every 4‑bit window */
        for (uint64_t pos = 0; pos + 4ULL <= len; ++pos) {
            uint64_t sub = (mask >> (len - 4ULL - pos)) & 0xFULL;
            if (sub == 0ULL || sub == 2ULL || sub == 10ULL) {
                bad = true;
                break;
            }
        }
        if (!bad) ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;

    /* Pre‑computed initial values for n = 4 … 8. */
    static seq_t base[9];
    static bool ready = false;
    if (!ready) {
        base[0] = base[1] = base[2] = base[3] = (seq_t)0;
        for (idx_t i = 4; i <= 8; ++i)
            base[i] = count_valid(i);
        ready = true;
    }

    if (n <= 8) return base[n];

    /* Ring buffer holding the last five values:
     * vals[0] = a(current-5), vals[1] = a(current-4), …, vals[4] = a(current-1). */
    seq_t vals[5];
    vals[0] = base[4];
    vals[1] = base[5];
    vals[2] = base[6];
    vals[3] = base[7];
    vals[4] = base[8];

    for (idx_t i = 9; i <= n; ++i) {
        seq_t new_val = vals[4] + vals[2] + vals[1] + vals[0];  /* a(i) */
        /* shift values left */
        vals[0] = vals[1];
        vals[1] = vals[2];
        vals[2] = vals[3];
        vals[3] = vals[4];
        vals[4] = new_val;
    }

    return vals[4];
}
