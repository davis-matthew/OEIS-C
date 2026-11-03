/*
 * A018872  a(n)^5 is smallest fifth power beginning with n.
 * https://oeis.org/A018872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest integer m such that m^5 starts * with the
 *            decimal representation of n. * This implementation uses a
 *            brute‑force search: for each n it * increments m until the
 *            leading digits of m^5 match n
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 8965 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
static seq_t leading_five_power_digits(uint64_t m, unsigned need)
{
    /* Multiply m five times; result stored in base 1e9 array.        */
    /* Maximum size: 5 * log10(10^9) = 45 decimal digits -> 5 words. */
    const uint32_t BASE = 1000000000U;          /* 1e9            */
    uint32_t d[10];
    size_t len = 1;
    d[0] = 1;                                  /* start from 1    */

    for (int i = 0; i < 5; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t prod = (uint64_t)d[j] * m + carry;
            d[j] = (uint32_t)(prod % BASE);
            carry = prod / BASE;
        }
        while (carry) {
            d[len++] = (uint32_t)(carry % BASE);
            carry /= BASE;
        }
    }

    /* Convert to decimal string.      */
    char buf[200] = {0};
    int pos = 0;
    /* Write most significant word without leading zeros. */
    pos += sprintf(buf + pos, "%u", d[len - 1]);
    /* Remaining words with exactly 9 digits. */
    for (int j = (int)len - 2; j >= 0; --j) {
        pos += sprintf(buf + pos, "%09u", d[j]);
    }

    /* Extract the first `need` digits. */
    if (need > (size_t)pos) need = (size_t)pos;
    char sub[32];
    memcpy(sub, buf, need);
    sub[need] = '\0';

    return (seq_t)strtoull(sub, NULL, 10);
}

/* ------------------------------------------------------------------ */
/* Helper: return the leading `need` decimal digits of m^5.             */

/* ------------------------------------------------------------------ */
/* Number of decimal digits of an unsigned integer.                     */
/* ------------------------------------------------------------------ */
static unsigned num_digits(idx_t x)
{
    unsigned d = 0;
    if (x == 0) return 1;
    while (x) {
        x /= 10;
        ++d;
    }
    return d;
}

/* ------------------------------------------------------------------ */
/* Main function required by the problem statement.                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Special case: 0^5 = 0 starts with 0. */
    if (n == 0) return (seq_t)0;

    unsigned need = num_digits(n);
    uint64_t m = 1;

    while (1) {
        seq_t lead = leading_five_power_digits(m, need);
        if (lead == n) return (seq_t)m;
        ++m;
    }
}
