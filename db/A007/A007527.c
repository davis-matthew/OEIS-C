/*
 * A007527  Numbers that are not the sum of 4 hexagonal numbers.
 * https://oeis.org/A007527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   Numbers that cannot be expressed as the sum of 4 hexagonal
 *            numbers. Computed by exhaustive search up to 1791
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


static uint64_t isqrt(uint64_t n) {
    if (n == 0) return 0;
    uint64_t x0 = n;
    uint64_t x1 = (x0 + 1) / 2;
    while (x1 < x0) {
        x0 = x1;
        x1 = (x0 + n / x0) / 2;
    }
    return x0;
}

static bool is_hexagonal(uint64_t num) {
    if (num == 0) return true;
    uint64_t discriminant = 1 + 8 * num;
    uint64_t sqrt_disc = isqrt(discriminant);
    if (sqrt_disc * sqrt_disc != discriminant) {
        return false;
    }
    if ((1 + sqrt_disc) % 4 != 0) {
        return false;
    }
    uint64_t n = (1 + sqrt_disc) / 4;
    return n > 0 && n * (2*n - 1) == num;
}

static bool can_sum_4_hexagonal(uint64_t k) {
    uint64_t hex[500];
    int hex_count = 0;
    hex[hex_count++] = 0;
    for (uint64_t n = 1; n * (2*n - 1) <= k; n++) {
        hex[hex_count++] = n * (2*n - 1);
    }
    for (int i = 0; i < hex_count; i++) {
        if (hex[i] > k) break;
        for (int j = 0; j < hex_count; j++) {
            if (hex[i] + hex[j] > k) break;
            for (int l = 0; l < hex_count; l++) {
                uint64_t sum3 = hex[i] + hex[j] + hex[l];
                if (sum3 > k) break;
                uint64_t remainder = k - sum3;
                if (is_hexagonal(remainder)) {
                    return true;
                }
            }
        }
    }
    return false;
}

seq_t a(idx_t n) {
    static seq_t *sequence = NULL;
    static idx_t sequence_len = 0;
    if (sequence == NULL) {
        sequence = (seq_t *)malloc(sizeof(seq_t) * 500);
        sequence_len = 0;
        for (uint64_t k = 1; k <= 1791; k++) {
            if (!can_sum_4_hexagonal(k)) {
                sequence[sequence_len++] = (seq_t)k;
            }
        }
    }
    if (n >= 1 && n <= sequence_len) {
        return sequence[n - 1];
    }
    return 0;
}
