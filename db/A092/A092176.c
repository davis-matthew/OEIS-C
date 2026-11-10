/*
 * A092176  a(n) = A067076(n) + A000079(n)/2 with A067076(0)=0.
 * https://oeis.org/A092176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = A067076(n) + A000079(n)/2 (integer division),
 *            A067076(0)=0; i.e. a(n) = A067076(n) + 2^(n-1) for n>=1, a(0)=0
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 34 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

static bool A067076_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool A067076_is_prime_big(seq_t n) {
    if (n < 2) return false;
    if (A067076_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t b = (seq_t)base_list[i];
        if (b >= n) continue;

        seq_t x = 1;
        seq_t pw = b % n;
        seq_t ex = d;
        while (ex > 0) {
            if (ex & 1) x = (x * pw) % n;
            pw = (pw * pw) % n;
            ex >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t A067076_a(idx_t n) {
    if (n == 0) return (seq_t)0;
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        seq_t candidate = 3 + ((seq_t)k) * 2;
        if (A067076_is_prime_big(candidate)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }
    return (seq_t)0;
}

static seq_t A000079_a(idx_t n)
{
    seq_t result = 1;
    seq_t base = 2;
    while (n > 0) {
        if (n & 1)
            result *= base;
        n >>= 1;
        if (n > 0)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t half = 0;
    if (n > 0) half = A000079_a(n - 1);
    return A067076_a(n) + half;
}
