/*
 * A208075  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 0 1 0 vertically.
 * https://oeis.org/A208075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed with a 3‑row DP. a(1) = 16 (number of 5‑bit rows
 *            that avoid 000 and 001 horizontally) a(2) = 256 (any pair of
 *            a(1) rows) For n ≥ 3: dynamic programming over pairs of
 *            consecutive rows, ensuring vertical triples never equal 000 or
 *            010
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑compute allowed 5‑bit rows and transition masks. */
    static bool ready = false;
    static bool allowed[32];
    static uint32_t trans_mask[32][32];

    if (!ready) {
        /* 1) allowed rows */
        for (unsigned r = 0; r < 32; ++r) {
            bool ok = true;
            for (unsigned j = 0; j <= 2; ++j) {
                unsigned bits = (r >> j) & 0x7;   /* three consecutive bits */
                if (bits == 0x0 || bits == 0x1) { /* 000 or 001 forbidden */
                    ok = false;
                    break;
                }
            }
            allowed[r] = ok;
        }

        /* 2) transition masks: for each (prev, cur) pair,
           determine all next rows that satisfy vertical constraints. */
        for (unsigned prev = 0; prev < 32; ++prev) {
            for (unsigned cur = 0; cur < 32; ++cur) {
                if (!allowed[prev] || !allowed[cur]) {
                    trans_mask[prev][cur] = 0u;
                    continue;
                }
                uint32_t mask = 0u;
                for (unsigned next = 0; next < 32; ++next) {
                    if (!allowed[next]) continue;
                    bool ok = true;
                    for (unsigned col = 0; col < 5; ++col) {
                        unsigned p = (prev >> col) & 1u;
                        unsigned c = (cur  >> col) & 1u;
                        unsigned n = (next >> col) & 1u;
                        if (p == 0 && c == 0 && n == 0) { ok = false; break; }
                        if (p == 0 && c == 1 && n == 0) { ok = false; break; }
                    }
                    if (ok) mask |= (1u << next);
                }
                trans_mask[prev][cur] = mask;
            }
        }
        ready = true;
    }

    /* Count of allowed rows. */
    static seq_t total_allowed = 0;
    if (total_allowed == 0) {
        for (unsigned r = 0; r < 32; ++r)
            if (allowed[r]) ++total_allowed;
    }

    if (n == 1) return total_allowed;
    if (n == 2) return total_allowed * total_allowed;

    /* DP over pairs of consecutive rows. */
    static const unsigned STATES = 32u * 32u;   /* 1024 */
    seq_t dp_prev[STATES];
    seq_t dp_next[STATES];

    /* Initialize with all allowed pairs for rows 1 and 2. */
    for (unsigned i = 0; i < STATES; ++i) dp_prev[i] = 0;
    for (unsigned prev = 0; prev < 32; ++prev) {
        if (!allowed[prev]) continue;
        for (unsigned cur = 0; cur < 32; ++cur) {
            if (!allowed[cur]) continue;
            dp_prev[prev * 32u + cur] = 1;
        }
    }

    /* Add rows 3 .. n */
    for (idx_t row = 3; row <= n; ++row) {
        for (unsigned i = 0; i < STATES; ++i) dp_next[i] = 0;

        for (unsigned idx = 0; idx < STATES; ++idx) {
            seq_t count = dp_prev[idx];
            if (count == 0) continue;
            unsigned prev = idx / 32u;
            unsigned cur  = idx % 32u;

            uint32_t mask = trans_mask[prev][cur];
            while (mask) {
                unsigned next = __builtin_ctz(mask);
                mask &= mask - 1;
                unsigned new_idx = cur * 32u + next;
                dp_next[new_idx] += count;
            }
        }
        /* swap dp_prev and dp_next */
        for (unsigned i = 0; i < STATES; ++i) dp_prev[i] = dp_next[i];
    }

    /* Sum all counts for final state. */
    seq_t result = 0;
    for (unsigned i = 0; i < STATES; ++i)
        result += dp_prev[i];

    return result;
}
