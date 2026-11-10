/*
 * A075041  Erroneous version of A006049.
 * https://oeis.org/A075041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   search: a(n) = n-th k with omega(k) = omega(k+1) (A006049), with
 *            k = 3, 4, 7, 8 omitted; omega found by trial division
 * Range:     i8 n <= 27, i16 n <= 27, i32 n <= 27, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int A075041_omega(uint64_t m) {
    int count = 0;
    uint64_t i;
    if (m <= 1) return 0;
    if (m % 2 == 0) {
        count++;
        while (m % 2 == 0) m /= 2;
    }
    for (i = 3; i <= m / i; i += 2) {
        if (m % i == 0) {
            count++;
            while (m % i == 0) m /= i;
        }
    }
    if (m > 1) count++;
    return count;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 1;
    int prev = A075041_omega(1);
    for (;;) {
        int cur = A075041_omega(k + 1);
        if (prev == cur && k != 3 && k != 4 && k != 7 && k != 8) {
            count++;
            if (count == n) return (seq_t)k;
        }
        prev = cur;
        k++;
    }
}
