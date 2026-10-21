/*
 * A217167  a(n) is the first digit (from the left) to appear two times in
 *          succession in the decimal representation of n^A217157(n).
 * https://oeis.org/A217167
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 12, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   get_digits, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:  a(n) is the first left‑most digit that occurs twice in a row
    in n^e, where e is the smallest positive integer such that n^e contains
    two consecutive equal decimal digits. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_digits.h"
#include "../utils/seq_mul.h"

/* Maximum digits we expect to need for n^e (sufficient for n ≤ 10^4). */
#define MAX_DIGITS 65536

seq_t a(idx_t n) {
    /* n^1 is the starting value (e = 1). */
    seq_t power = (seq_t)n;
    uint8_t digits[MAX_DIGITS];
    size_t len;

    for (idx_t exp = 1;; ++exp) {
        /* Obtain the decimal digits of the current power. */
        get_digits(power, digits, &len);
        if (len > MAX_DIGITS) {          /* just in case */
            /* If more digits are needed, allocate a buffer dynamically. */
            uint8_t *tmp = (uint8_t*)malloc(len * sizeof(uint8_t));
            if (!tmp) return 0;          /* fallback on failure */
            get_digits(power, tmp, &len);
            /* Reverse to have most significant digit first. */
            for (size_t i = 0, j = len - 1; i < j; ++i, --j) {
                uint8_t t = tmp[i];
                tmp[i] = tmp[j];
                tmp[j] = t;
            }
            /* Scan for the first adjacent pair. */
            for (size_t i = 0; i + 1 < len; ++i)
                if (tmp[i] == tmp[i + 1])
                    return (seq_t)tmp[i];
            free(tmp);
        } else {
            /* Reverse the array so that digits[0] is the most significant. */
            for (size_t i = 0, j = len - 1; i < j; ++i, --j) {
                uint8_t t = digits[i];
                digits[i] = digits[j];
                digits[j] = t;
            }

            /* Scan for the first adjacent pair. */
            for (size_t i = 0; i + 1 < len; ++i)
                if (digits[i] == digits[i + 1])
                    return (seq_t)digits[i];
        }

        /* No pair found yet – increase the exponent. */
        power = seq_mul(power, (seq_t)n);
    }
}
