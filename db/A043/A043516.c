/*
 * A043516  Numbers having four 6's in base 10.
 * https://oeis.org/A043516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer containing exactly four decimal
 *            digit 6
 * Range:     u8 none, u16 n <= 11, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow9_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow9_uint64.h"

/* binomial coefficient C(n,k) using 128‑bit intermediate */
static uint64_t binomial_uint64(uint64_t n, uint64_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    __int128 res = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return (uint64_t)res;
}

/* number of d‑digit integers that contain exactly four 6’s */
static uint64_t count_exact_sixes_u64(uint64_t d) {
    if (d < 4) return 0;

    uint64_t total = 0;
    /* first digit is 6 */
    if (d - 1 >= 3) {
        uint64_t c1 = binomial_uint64(d - 1, 3);
        c1 = (uint64_t)((__int128)c1 * pow9_uint64((d - 1) - 3));
        total += c1;
    }
    /* first digit is 1–9 except 6 (8 choices) */
    if (d - 1 >= 4) {
        uint64_t c2 = binomial_uint64(d - 1, 4);
        c2 = (uint64_t)((__int128)c2 * pow9_uint64((d - 1) - 4));
        c2 *= 8; /* 8 possible leading digits (1‑9 except 6) */
        total += c2;
    }
    return total;
}

/* main function requested by the interface */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for n < 1 */

    /* 1. Find the digit length d that contains the n-th term */
    uint64_t target = (uint64_t)n;
    uint64_t cumulative = 0;
    uint64_t d = 4;
    for (;;) {
        uint64_t cnt = count_exact_sixes_u64(d);
        if (cumulative + cnt >= target) break;
        cumulative += cnt;
        ++d;
    }
    uint64_t offset = target - cumulative; /* 1‑based offset within d‑digit numbers */

    /* 2. Construct the offset‑th number of length d */
    seq_t result = (seq_t)0;
    uint64_t remaining_sixes = 4;
    for (uint64_t pos = 0; pos < d; ++pos) {
        uint64_t rem_pos = d - pos - 1;
        uint64_t start_digit = (pos == 0) ? 1 : 0;
        for (uint64_t digit = start_digit; digit <= 9; ++digit) {
            uint64_t new_six = remaining_sixes;
            if (digit == 6) {
                if (remaining_sixes == 0) continue;
                new_six = remaining_sixes - 1;
            }
            if (new_six > rem_pos) continue;

            uint64_t branch =
                binomial_uint64(rem_pos, new_six) *
                pow9_uint64(rem_pos - new_six);

            if (offset > branch) {
                offset -= branch;
                continue;
            }

            /* choose this digit */
            result = result * (seq_t)10 + (seq_t)digit;
            remaining_sixes = new_six;
            break;
        }
    }
    return result;
}
