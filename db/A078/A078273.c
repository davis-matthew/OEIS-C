/*
 * A078273  Smallest multiple of n other than n using only the digits of n (no
 *          limit on frequency).
 * https://oeis.org/A078273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 110, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest positive multiple of n that is not n itself
   and whose decimal digits are all among the digits of n.
   If n contains a zero digit then a(n) = 10 * n.
   Otherwise we test multiples k*n for k = 2,3,…,10^d+1
   where d is the number of digits of n. */
/* The algorithm is brute‑force but guaranteed to finish
   because a(n) ≤ (10^d + 1) * n. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Convert idx_t to seq_t only for returning */
    const idx_t nn = n;          /* original index (n) */
    /* Count digits, detect zero digit */
    bool has_zero = false;
    bool allowed[10] = {0};
    idx_t tmp = nn;
    size_t d = 0;
    while (tmp > 0) {
        unsigned digit = (unsigned)(tmp % 10);
        allowed[digit] = true;
        if (digit == 0)
            has_zero = true;
        tmp /= 10;
        ++d;
    }

    /* If zero digit exists, smallest multiple containing only n's digits is 10*n */
    if (has_zero) {
        return ((seq_t)nn) * 10;
    }

    /* Precompute 10^d */
    uint64_t pow10 = 1;
    for (size_t i = 0; i < d; ++i)
        pow10 *= 10;

    /* Test multiples k*n for k = 2 .. 10^d + 1 */
    for (uint64_t k = 2; k <= pow10 + 1; ++k) {
        uint64_t prod = (uint64_t)nn * k;                     /* product fits in 64‑bit (nn≤10⁴, k≤10⁵+1) */
        uint64_t m = prod;
        bool ok = true;
        while (m > 0) {
            unsigned dgt = (unsigned)(m % 10);
            if (!allowed[dgt]) { ok = false; break; }
            m /= 10;
        }
        if (ok) {
            return (seq_t)prod;
        }
    }

    /* Fallback:  (10^d + 1) * n (should never be reached) */
    return ((seq_t)nn) * (pow10 + 1);
}
