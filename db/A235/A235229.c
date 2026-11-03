/*
 * A235229  Numbers whose sum of digits is 20.
 * https://oeis.org/A235229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 1000 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function computes the nth positive integer whose decimal digit sum equals 20.
   Uses binary search combined with a digit‑DP (count_leq) to find the value. */

#include <stddef.h>
#include <stdint.h>

static void get_digits_msf(seq_t x, uint8_t *digits, size_t *len)
{
    size_t pos = 0;

    if (x == 0) {
        digits[0] = 0;
        *len = 1;
        return;
    }

    /* Collect digits least‑significant first */
    while (x != 0) {
        digits[pos++] = (uint8_t)(x % (seq_t)10);
        x /= (seq_t)10;
    }

    /* Reverse to most‑significant first */
    for (size_t i = 0; i < pos / 2; ++i) {
        uint8_t tmp = digits[i];
        digits[i] = digits[pos - 1 - i];
        digits[pos - 1 - i] = tmp;
    }

    *len = pos;
}

#define TARGET_SUM 20
#define MAX_DIGITS 256  /* safe upper bound for the digit length needed */

/* Count numbers ≤ x whose decimal digit sum equals TARGET_SUM */
static seq_t count_leq(seq_t x)
{
    uint8_t digits[MAX_DIGITS];
    size_t len;

    get_digits_msf(x, digits, &len);

    seq_t dp_tight[TARGET_SUM + 1] = {0};
    seq_t dp_lax[TARGET_SUM + 1]   = {0};
    seq_t next_tight[TARGET_SUM + 1];
    seq_t next_lax[TARGET_SUM + 1];

    dp_tight[0] = 1;  /* empty prefix, sum 0, still tight */

    for (size_t pos = 0; pos < len; ++pos) {
        uint8_t limit = digits[pos];

        for (int s = 0; s <= TARGET_SUM; ++s) {
            next_tight[s] = 0;
            next_lax[s]   = 0;
        }

        /* transitions from tight states */
        for (int s = 0; s <= TARGET_SUM; ++s) {
            seq_t cur = dp_tight[s];
            if (!cur) continue;
            for (uint8_t d = 0; d <= limit; ++d) {
                int ns = s + d;
                if (ns > TARGET_SUM) break;
                if (d == limit)
                    next_tight[ns] += cur;
                else
                    next_lax[ns]   += cur;
            }
        }

        /* transitions from lax states */
        for (int s = 0; s <= TARGET_SUM; ++s) {
            seq_t cur = dp_lax[s];
            if (!cur) continue;
            for (uint8_t d = 0; d <= 9; ++d) {
                int ns = s + d;
                if (ns > TARGET_SUM) break;
                next_lax[ns] += cur;
            }
        }

        /* copy back for next iteration */
        for (int s = 0; s <= TARGET_SUM; ++s) {
            dp_tight[s] = next_tight[s];
            dp_lax[s]   = next_lax[s];
        }
    }

    return dp_tight[TARGET_SUM] + dp_lax[TARGET_SUM];
}

seq_t a(idx_t n)
{
    seq_t low  = 0;
    seq_t high = 1;
    seq_t target = (seq_t)n;

    /* Exponential search to find an upper bound */
    while (count_leq(high) < target)
        high = high * 2;

    /* Binary search for the smallest x with count ≥ target */
    while (low < high) {
        seq_t mid = (low + high) / 2;
        if (count_leq(mid) >= target)
            high = mid;
        else
            low = mid + 1;
    }

    return low;
}
