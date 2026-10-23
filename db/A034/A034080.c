/*
 * A034080  Decimal part of n-th root of a(n) starts with digit 3.
 * https://oeis.org/A034080
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceil(13^n / 10^n) for n ≥ 4, with a(2) = 11, a(3) = 13.
 *            Computation is performed with a small decimal base‑9 big‑integer
 *            that fits comfortably in memory for all n used in the tests
 * Range:     u8 n <= 21, u16 n <= 42, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Base 10^9 big‑integer representation (max ~2000 chunks for n≤10000). */
#define BASE 1000000000U
#define MAX_CHUNKS 2000

static seq_t
big_pow13_then_divide(idx_t n)
{
    /* Handle the exceptional small indices explicitly. */
    if (n == 2)
        return 11;
    if (n == 3)
        return 13;
    if (n < 2)
        return 0;           /* Undefined for the offset; return 0. */

    /* 13^n in base BASE. */
    uint32_t digits[MAX_CHUNKS];
    int len = 1;                 /* Current number of used chunks. */
    digits[0] = 1;

    /* Repeated multiplication by 13.  (n ≤ 10000 → about 1³*10⁴ operations.) */
    for (idx_t i = 0; i < n; ++i) {
        uint64_t carry = 0;
        for (int j = 0; j < len; ++j) {
            uint64_t prod = (uint64_t)digits[j] * 13 + carry;
            digits[j]   = (uint32_t)(prod % BASE);
            carry       = prod / BASE;
        }
        if (carry) {
            digits[len++] = (uint32_t)carry;
        }
    }

    /* Compute floor(13^n / 10^n) by removing the last n decimal digits.  */
    int shift_chunks = (int)(n / 9);
    int shift_digits = (int)(n % 9);
    int qlen = len - shift_chunks;      /* Quotient chunk length. */
    if (qlen <= 0)
        qlen = 0;                       /* Should not happen for n≥4. */

    uint32_t quo[MAX_CHUNKS];
    int power10_r = 1;
    for (int t = 0; t < shift_digits; ++t)
        power10_r *= 10;                /* 10^shift_digits, shift_digits ≤ 8. */

    uint64_t carry = 0;
    /* Divide the remaining high part by 10^shift_digits. */
    for (int idx = len - 1; idx >= shift_chunks; --idx) {
        uint64_t acc  = (uint64_t)carry * BASE + digits[idx];
        uint32_t qdig = (uint32_t)(acc / power10_r);
        carry          = (uint32_t)(acc % power10_r);
        quo[idx - shift_chunks] = qdig;
    }

    /* Remove possible leading zeros in the quotient. */
    while (qlen > 0 && quo[qlen - 1] == 0)
        --qlen;

    /* Convert the quotient into a seq_t value (fits in seq_t for all tested n). */
    seq_t result = 0;
    for (int i = qlen - 1; i >= 0; --i)
        result = result * BASE + quo[i];

    /* Remainder after division is always non‑zero for n ≥ 4, so ceil = floor + 1. */
    return result + 1;
}

seq_t a(idx_t n)
{
    return big_pow13_then_divide(n);
}
