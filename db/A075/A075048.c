/*
 * A075048  10-smooth numbers that show their prime factors.
 * https://oeis.org/A075048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) is computed by generating all 10‑smooth numbers in
 *            increasing order and selecting those whose decimal digits are in
 *            {1,2,3,5,7} and for which the set of prime divisors {2,3,5,7}
 *            matches the presence of the corresponding digits
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   digit_counts, mul_small, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/mul_small.h"
#include "../utils/digit_counts.h"

static bool satisfies(seq_t x)
{
    unsigned char cnt[10] = {0};
    digit_counts(x, cnt);
    if (cnt[0] || cnt[4] || cnt[6] || cnt[8] || cnt[9])
        return false;
    bool div2 = (x % 2 == 0);
    bool div3 = (x % 3 == 0);
    bool div5 = (x % 5 == 0);
    bool div7 = (x % 7 == 0);
    if ((cnt[2] > 0) != div2) return false;
    if ((cnt[3] > 0) != div3) return false;
    if ((cnt[5] > 0) != div5) return false;
    if ((cnt[7] > 0) != div7) return false;
    return true;
}

static void ensure_cap(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed > *cap) {
        size_t new_cap = *cap > 0 ? *cap * 2 : 64;
        while (new_cap < needed)
            new_cap <<= 1;
        *arr = seq_realloc(*arr, *cap, new_cap);
        *cap = new_cap;
    }
}

/* State for generating 10‑smooth numbers */
static seq_t *ugly = NULL;
static size_t ugly_len = 0, ugly_cap = 0;
static idx_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;

/* State for storing the sequence */
static seq_t *valid = NULL;
static size_t valid_len = 0, valid_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety: n≥1 in tests */

    if (ugly == NULL) {
        ugly_len = 1;
        ugly_cap = 64;
        ugly = seq_alloc(ugly_cap);
        ugly[0] = (seq_t)1;

        valid_len = 1;
        valid_cap = 64;
        valid = seq_alloc(valid_cap);
        valid[0] = (seq_t)1;
    }

    size_t need = (size_t)n;
    while (valid_len < need) {
        seq_t cand2 = mul_small(ugly[i2], 2);
        seq_t cand3 = mul_small(ugly[i3], 3);
        seq_t cand5 = mul_small(ugly[i5], 5);
        seq_t cand7 = mul_small(ugly[i7], 7);

        seq_t next = cand2;
        if (cand3 < next) next = cand3;
        if (cand5 < next) next = cand5;
        if (cand7 < next) next = cand7;

        ensure_cap(ugly_len + 1, &ugly_cap, &ugly);
        ugly[ugly_len++] = next;

        if (cand2 == next) ++i2;
        if (cand3 == next) ++i3;
        if (cand5 == next) ++i5;
        if (cand7 == next) ++i7;

        if (satisfies(next)) {
            ensure_cap(valid_len + 1, &valid_cap, &valid);
            valid[valid_len++] = next;
        }
    }

    return valid[n - 1];
}
