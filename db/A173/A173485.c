/*
 * A173485  a(n) = the smallest (n+2)-digit number ending in n+1 zeros that is
 *          divisible by n, else 0.
 * https://oeis.org/A173485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A173485:
   a(n) = the smallest (n+2)-digit number ending in n+1 zeros that
   is divisible by n; returns 0 if no such number exists. */
#include <stdint.h>
#include <stdbool.h>

/* Required helper functions are provided by the build environment. */
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* seq_t and idx_t are defined by the build; this function uses only
   integer arithmetic and the helper routines above. */
seq_t a(idx_t n)
{
    /* Guard against n == 0 (outside the OEIS offset). */
    if (n == 0)
        return 0;

    /* Compute 10^(n+1) mod n using modular exponentiation. */
    seq_t t_mod = powmod(10, (uint64_t)(n + 1), (seq_t)n);
    /* t_mod < n, so safe to cast to idx_t. */
    idx_t t = (idx_t)t_mod;

    /* Search for the smallest digit d (1..9) such that d * t ≡ 0 (mod n). */
    for (int d = 1; d <= 9; ++d) {
        if (((t * (idx_t)d) % n) == 0) {
            /* The desired number is d * 10^(n+1). */
            seq_t base = pow10(n + 1);
            return mul_small(base, d);
        }
    }

    /* No digit found ⇒ return 0. */
    return 0;
}
