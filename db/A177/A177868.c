/*
 * A177868  a(n) = number of 2-digit primes with digit sum n, where n runs
 *          through the non-multiples of 3 in the range [2..17].
 * https://oeis.org/A177868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = number of 2‑digit primes with digit sum equal to the n‑th
 *            non‑multiple of 3 in the range [2..17]. This implementation
 *            precomputes the prime counts for all possible digit sums (0..18)
 *            and performs a table lookup
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const idx_t sums[] = {2,4,5,7,8,10,11,13,14,16,17};
static seq_t digit_sum_counts[19];
static bool initialized = false;

static bool is_prime_uint32(uint32_t v) {
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    if (v % 3 == 0) return v == 3;
    for (uint32_t d = 5; (uint64_t)d*d <= v; d += 6) {
        if (v % d == 0 || v % (d+2) == 0) return false;
    }
    return true;
}

static void init_counts(void) {
    for (idx_t i = 0; i <= 18; ++i) digit_sum_counts[i] = 0;
    for (uint32_t num = 10; num < 100; ++num) {
        if (!is_prime_uint32(num)) continue;
        uint32_t s = (num / 10) + (num % 10);
        if (s <= 18) {
            digit_sum_counts[s] = digit_sum_counts[s] + 1;
        }
    }
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) init_counts();
    if (n < 1) return 0;                       /* undefined for n < 1 */
    idx_t idx = n - 1;
    if (idx >= (idx_t)(sizeof(sums)/sizeof(sums[0]))) return 0; /* out of range */
    idx_t sum = sums[idx];
    return digit_sum_counts[sum];
}
