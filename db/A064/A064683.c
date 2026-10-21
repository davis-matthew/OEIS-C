/*
 * A064683  Let n(s) be the number formed from n by inserting s 0's between
 *          each digit, e.g. 123(2) is 1002003; sequence gives numbers n such
 *          that n(s) is divisible by n for some s>0.
 * https://oeis.org/A064683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   modmul_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A064683
 *  Implementation: Linear search for the nth integer that has a
 *  power‑splitting representation n(s) divisible by n for some s>0.
 *  The algorithm tests each candidate n by trying all s=1…n
 *  (sufficient for the required range) and returns the nth such n.
 */
 
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/modmul_seq.h"

/* Return true iff n has the required property: ∃s>0 with n(s)%n==0. */
static bool has_property(seq_t n)
{
    if (n == (seq_t)1) return true;          /* 1 always satisfies the condition */

    /* Extract decimal digits (most significant first) */
    uint8_t digits[64];      /* enough for up to 39 decimal digits of 128‑bit */
    unsigned int len = 0;
    seq_t tmp = n;
    while (tmp > (seq_t)0) {
        digits[len++] = (uint8_t)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }
    /* Reverse to MSB→LSB order */
    for (unsigned int i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* Iterate over s = 1…n.  For each s compute pow10 = 10^(s+1) mod n. */
    seq_t pow10 = (seq_t)(10 % n);   /* 10^(1) mod n */
    for (idx_t s = 1; s <= n; ++s) {
        /* Update to 10^(s+1) mod n */
        pow10 = modmul_seq(pow10, (seq_t)10, n);

        /* Compute n(s) modulo n by processing digits from left to right */
        seq_t val = (seq_t)0;
        for (unsigned int i = 0; i < len; ++i) {
            val = modmul_seq(val, pow10, n);
            val += (seq_t)digits[i];
            if (val >= n) val -= n;       /* equivalent to val% n */
        }
        if (val == (seq_t)0) return true;
    }
    return false;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = (seq_t)1;

    while (count < n) {
        if (has_property(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
    /* Unreachable but satisfies compiler */
    return (seq_t)0;
}
