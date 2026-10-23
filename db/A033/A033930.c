/*
 * A033930  Base 10 digital convolution sequence.
 * https://oeis.org/A033930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; for n > 0, let n = Σ_{i=0}^{k} d_i × 10^i, then a(n) =
 *            Σ_{i=0}^{k} d_i × a(i)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t memo[21];
static bool seen[21] = {false};

static void ensure(idx_t idx) {
    if (idx >= 21) return;            /* safety guard – should not happen */
    if (seen[idx]) return;
    if (idx == 0) {
        memo[0] = (seq_t)1;
        seen[0] = true;
    } else {
        idx_t temp = idx;
        idx_t pos  = 0;
        seq_t sum  = 0;
        while (temp > 0) {
            uint8_t digit = temp % 10;
            ensure(pos);
            sum += (seq_t)digit * memo[pos];
            temp /= 10;
            pos++;
        }
        memo[idx] = sum;
        seen[idx] = true;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t sum = 0;
    idx_t temp = n;
    idx_t pos = 0;
    while (temp > 0) {
        uint8_t digit = temp % 10;
        ensure(pos);
        sum += (seq_t)digit * memo[pos];
        temp /= 10;
        pos++;
    }
    return sum;
}
