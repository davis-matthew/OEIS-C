/*
 * A043801  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 3 mod 8.
 * https://oeis.org/A043801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043801: nth integer whose base‑3 representation has a number of runs ≡ 3 (mod 8). */

#include <stddef.h>

/* Return 1 if n has a number of runs in base‑3 congruent to 3 mod 8, 0 otherwise. */
static inline int runs_is_3_mod_8(seq_t n)
{
    if (n == 0) return 0;
    seq_t x = n;
    int prev = -1;
    int runs = 0;
    while (x > 0) {
        seq_t dseq = x % 3;                 /* digit is 0,1 or 2 */
        int digit = (int)(long long)dseq;   /* safe cast – the value is 0‑2 */
        if (digit != prev) {
            runs++;
            prev = digit;
        }
        x = x / 3;
    }
    return (runs & 7) == 3;                  /* runs % 8 == 3 */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* a(0) is not defined; sequence is 1‑based */
    idx_t counted = 0;
    seq_t candidate = 1;
    while (1) {
        if (runs_is_3_mod_8(candidate)) {
            counted++;
            if (counted == n) return candidate;
        }
        candidate = candidate + 1;
    }
}
