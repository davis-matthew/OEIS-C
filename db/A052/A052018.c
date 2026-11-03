/*
 * A052018  Numbers k with the property that the sum of the digits of k is a
 *          substring of k.
 * https://oeis.org/A052018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 3317, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9489 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the sum of the digits of k is a decimal
   substring of the decimal representation of k.  The function
   enumerates integers and returns the n-th such number. */

#include <stdbool.h>
#include <stdint.h>

/* Helper to test the property for a single number. */
static bool has_sum_substring(seq_t k)
{
    /* Special case: 0.  Sum of digits = 0, which is a substring. */
    if (k == (seq_t)0) {
        return true;
    }

    /* Extract digits of k from most significant to least significant
       and compute the digit sum. */
    int digits[512];
    int len_k = 0;
    seq_t tmp = k;
    int sum = 0;

    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);
        digits[len_k++] = d;
        sum += d;
        tmp /= (seq_t)10;
    }

    /* Reverse digits to get most‑significant‑first order. */
    for (int i = 0; i < len_k / 2; ++i) {
        int t = digits[i];
        digits[i] = digits[len_k - 1 - i];
        digits[len_k - 1 - i] = t;
    }

    /* Convert the digit sum to an array of its decimal digits. */
    int sum_digits[512];
    int len_s = 0;
    if (sum == 0) {
        sum_digits[0] = 0;
        len_s = 1;
    } else {
        int s = sum;
        while (s > 0) {
            sum_digits[len_s++] = s % 10;
            s /= 10;
        }
        /* Reverse to most‑significant‑first. */
        for (int i = 0; i < len_s / 2; ++i) {
            int t = sum_digits[i];
            sum_digits[i] = sum_digits[len_s - 1 - i];
            sum_digits[len_s - 1 - i] = t;
        }
    }

    if (len_s > len_k) return false;  /* impossible to appear */

    /* Scan for the substring. */
    for (int i = 0; i + len_s <= len_k; ++i) {
        bool ok = true;
        for (int j = 0; j < len_s; ++j) {
            if (digits[i + j] != sum_digits[j]) {
                ok = false;
                break;
            }
        }
        if (ok) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 0;

    while (true) {
        if (has_sum_substring(k)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
