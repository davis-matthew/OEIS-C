/*
 * A055510  Largest odd prime in sequence obtained in 3x+1 (or Collatz)
 *          problem starting at n, or 0 if no such prime is found.
 * https://oeis.org/A055510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest odd prime appearing in the Collatz sequence
 *            starting from n (or 0 if none). The Collatz iteration is x → x/2
 *            if x even, otherwise x → 3x+1
 * Range:     u8 n <= 26, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper isPrimeGeneric is provided by the OEIS build; it
   correctly tests primality of a seq_t value. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    seq_t x      = (seq_t)n;   /* start of the Collatz sequence */
    seq_t best   = 0;          /* largest odd prime seen so far */

    while (true)
    {
        /* Test odd numbers for primality */
        if ((x & 1) && isPrimeGeneric(x) && x > best)
            best = x;

        /* Stop once we reach 1 (which is not prime) */
        if (x == 1)
            break;

        /* Next Collatz step */
        if ((x & 1) == 0)      /* even */
            x >>= 1;
        else                   /* odd */
            x = 3 * x + 1;
    }

    return best;
}
