/*
 * A088886  Minimum number of consecutive previous nonnegative integers to n
 *          that must be concatenated together in ascending order such that n
 *          divides the concatenated term, or zero if n divides no such
 *          concatenation.
 * https://oeis.org/A088886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A088886:
   For n > 0, find the smallest k ≥ 1 such that the decimal
   concatenation of (n‑k), (n‑k+1), …, (n‑1) is divisible by n.
   If no such k exists, return 0.  The algorithm builds the
   concatenation modulo n backwards from n‑1 to 0. */
#include <stdint.h>
#include "../utils/mod_mul.h"

/* Count decimal digits of a non‑negative integer. Handles 0 → 1 digit. */
static inline int digits_of(idx_t x)
{
    if (x == 0) return 1;
    int d = 0;
    while (x > 0) {
        x /= 10;
        ++d;
    }
    return d;
}

/* Compute 10^e (mod mod). */
static inline uint64_t pow10_mod(uint64_t e, uint64_t mod)
{
    uint64_t r = 1 % mod;
    for (uint64_t i = 0; i < e; ++i) {
        r = mod_mul(r, 10ULL, mod);
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0 */

    uint64_t mod         = 0;          /* remainder of current suffix */
    uint64_t pow10_curr  = 1 % n;      /* 10^len_of_suffix modulo n */

    for (idx_t s = n - 1;; --s) {
        int ds = digits_of(s);
        uint64_t pow10_ds = pow10_mod((uint64_t)ds, n);

        uint64_t tmp_mul = mod_mul((s % n), pow10_curr, n);

        /* tmp_mul + mod modulo n, avoiding 128‑bit arithmetic */
        uint64_t new_mod;
        if (tmp_mul >= n - mod)
            new_mod = tmp_mul - (n - mod);
        else
            new_mod = tmp_mul + mod;

        mod = new_mod;
        pow10_curr = mod_mul(pow10_curr, pow10_ds, n);

        if (new_mod == 0)
            return (seq_t)(n - s);  /* k = n - s */

        if (s == 0) break;
    }
    return 0; /* no suitable concatenation */
}
