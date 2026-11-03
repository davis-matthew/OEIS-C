/*
 * A217707  Numbers k such that both 4*k-1 and 4*k+1 are composite.
 * https://oeis.org/A217707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A217707: numbers k such that both 4*k-1 and 4*k+1 are composite. */
/* Uses a simple search, testing each candidate sequentially. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}
#include <stdbool.h>

static bool k_is_valid(idx_t k)
{
    seq_t a = (seq_t)4 * k - 1;
    seq_t b = (seq_t)4 * k + 1;
    /* composite means >1 and not prime */
    return !isPrimeGeneric(a) && !isPrimeGeneric(b);
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 2;         /* k=1 gives 3 and 5, both prime – skip */
    for (;; ++k)
    {
        if (k_is_valid(k))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
