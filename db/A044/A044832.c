/*
 * A044832  Positive integers having more base-6 runs of even length than odd.
 * https://oeis.org/A044832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Positive integers having more base‑6 runs of even length than odd.
   Computed by searching integers incrementally. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/append_candidate.h"

static inline bool qualifies(seq_t x)
{
    idx_t even = 0, odd = 0;
    int last_digit = -1;
    int run_len = 0;
    bool first = true;
    seq_t t = x;

    if (t == 0) {
        /* base‑6 representation of 0 is "0" – one odd run */
        odd = 1;
    } else {
        while (t > 0) {
            int d = (int)(t % 6);  /* digit in base‑6 */
            t /= 6;
            if (first) {
                last_digit = d;
                run_len = 1;
                first = false;
            } else if (d == last_digit) {
                run_len++;
            } else {
                if (run_len & 1) odd++; else even++;
                last_digit = d;
                run_len = 1;
            }
        }
        if (run_len & 1) odd++; else even++;
    }

    return even > odd;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 1;

    if (n == 0) return 0;          /* undefined for n == 0, return 0 as safety */

    while (cache_len < n) {
        if (qualifies(last_checked))
            append_candidate(&cache, &cache_len, &cache_cap, last_checked);
        last_checked++;
    }

    return cache[n - 1];
}
