/*
 * A044916  Numbers whose base-15 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation method: For each integer n ≥ 1, convert n to base‑15 and
   verify that the lengths of consecutive runs of identical digits
   alternate odd, even, odd, … .  The sequence is generated on demand
   with memoisation.  This technique is fast enough for n up to
   about 10 000. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if the base‑15 representation of x satisfies the
   alternating run‑length condition. */
static bool valid_base15(seq_t x)
{
    if (x == 0)          /* 0 is not part of the sequence (offset ≥ 1) */
        return false;

    /* Convert x to base‑15 digits.  Little‑endian order. */
    uint8_t digits[128];
    int len = 0;
    seq_t y = x;
    while (y != 0) {
        digits[len++] = (uint8_t)(y % 15);
        y = y / 15;
    }

    int expected_parity = 1;   /* 1 → odd, 0 → even ; first run must be odd */
    int run_len = 1;

    /* Scan digits from most significant to least significant. */
    for (int i = len - 2; i >= 0; --i) {
        if (digits[i] == digits[i + 1]) {
            ++run_len;          /* extend current run */
        } else {
            /* check that the finished run has the required parity */
            if ((run_len & 1) != expected_parity)
                return false;
            expected_parity ^= 1;   /* toggle to next expected parity */
            run_len = 1;            /* start new run */
        }
    }

    /* check the last run */
    if ((run_len & 1) != expected_parity)
        return false;

    return true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* Undefined for n < 1; not used by the test harness */

    static seq_t *cache   = NULL;   /* memoised values */
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static seq_t next_val = 1;      /* next integer to test */

    while (cache_len < n) {
        seq_t candidate = next_val;
        if (valid_base15(candidate)) {
            /* grow the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { /* Failure to allocate memory */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
        ++next_val;
    }
    return cache[n - 1];
}
