/*
 * A043455  Numbers having three 0's in base 9.
 * https://oeis.org/A043455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   power8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having three 0's in base 9 (OEIS A043455).
   The nth term is the n-th integer whose base‑9 representation
   contains exactly three digits 0; the sequence starts with
   729, 1458, 2187, 2916, … .
*/

#include <stdint.h>
#include "../utils/power8.h"

static inline uint64_t comb_int(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

static inline uint64_t count_seq(int rem, int z)
{
    if (z < 0 || z > rem) return 0;
    return comb_int(rem, z) * power8(rem - z);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0, safeguard */

    idx_t idx = n;                  /* 1‑based index */
    uint64_t cumulative = 0;
    uint64_t d = 4;                 /* minimal length of base‑9 representation */

    /* find the digit‑length d that contains the nth term */
    while (1) {
        uint64_t cnt_len = (comb_int(d, 3) - comb_int(d - 1, 2)) * power8(d - 3);
        if (cumulative + cnt_len >= idx) break;
        cumulative += cnt_len;
        ++d;
    }

    uint64_t offset = idx - cumulative;  /* 1‑based offset within this length */
    int digit[64];                       /* holds the base‑9 digits */
    int zeros_needed = 3;
    uint64_t k = offset;

    for (uint64_t pos = 0; pos < d; ++pos) {
        int min_digit = (pos == 0) ? 1 : 0;
        for (int dig = min_digit; dig <= 8; ++dig) {
            int new_zeros = zeros_needed - (dig == 0 ? 1 : 0);
            if (new_zeros < 0) continue;
            int rem = d - pos - 1;
            uint64_t cnt = count_seq(rem, new_zeros);
            if (cnt < k) {
                k -= cnt;            /* this block is skipped */
                continue;
            }
            /* pick this digit */
            digit[pos] = dig;
            zeros_needed = new_zeros;
            break;
        }
    }

    /* convert from base‑9 to decimal */
    seq_t value = 0;
    for (uint64_t pos = 0; pos < d; ++pos) {
        value = value * 9 + (seq_t)digit[pos];
    }
    return value;
}
