/*
 * A093640  Number of divisors of n whose binary representation is contained
 *          in that of n.
 * https://oeis.org/A093640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of divisors d of n such that the binary *
 *            representation of d occurs as a contiguous substring of the
 *            binary * representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 7706 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static void to_bin(uint64_t x, char *buf, int *len)
{
    /* Convert x to binary string in buf[0..*len-1] (no leading zeros),
     * buf is assumed large enough (at least 65). */
    if (x == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    int p = 0;
    while (x) {
        buf[p++] = (x & 1U) ? '1' : '0';
        x >>= 1;
    }
    /* Reverse to get most significant bit first */
    for (int i = 0; i < p / 2; ++i) {
        char t = buf[i];
        buf[i] = buf[p - 1 - i];
        buf[p - 1 - i] = t;
    }
    *len = p;
}

static bool contains_bin(uint64_t d, const char *n_bin, int n_len)
{
    /* Check whether binary string of d is a substring of n_bin. */
    char d_bin[65];
    int d_len, i, j, k;
    to_bin(d, d_bin, &d_len);
    if (d_len > n_len) return false;
    for (i = 0; i <= n_len - d_len; ++i) {
        for (j = 0; j < d_len; ++j) {
            if (n_bin[i + j] != d_bin[j]) break;
        }
        if (j == d_len) return true;
    }
    return false;
}

/* Determine a(n) for n >= 1 */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;          /* work in native 64‑bit */
    char n_bin[65];
    int n_len;
    to_bin(N, n_bin, &n_len);

    seq_t count = 0;
    for (uint64_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            uint64_t d1 = i;
            uint64_t d2 = N / i;
            if (contains_bin(d1, n_bin, n_len))
                count++;
            if (d1 != d2 && contains_bin(d2, n_bin, n_len))
                count++;
        }
    }
    return count;
}
