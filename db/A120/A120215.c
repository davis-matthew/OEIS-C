/*
 * A120215  Start with 1057 and repeatedly reverse the digits and add 2 to get
 *          the next term.
 * https://oeis.org/A120215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: T(1057, 2)
   a(1) = 1057
   a(n) = reverse_decimal(a(n-1)) + 2   for n > 1
   The sequence is periodic with period 45, so a(n+45) = a(n).  */

#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------
   Local helper that reverses the decimal digits of a seq_t value.
   ------------------------------------------------------------------ */
static seq_t reverse_decimal_local(seq_t x)
{
    seq_t r = (seq_t)0;
    while (x != (seq_t)0) {
        r = r * (seq_t)10 + (x % (seq_t)10);
        x /= (seq_t)10;
    }
    return r;
}

/* ------------------------------------------------------------------
   The sequence function as required.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Cache the 45‑term cycle once, available for all calls. */
    static bool init = false;
    static seq_t cache[45];

    if (!init) {
        seq_t t = (seq_t)1057;
        cache[0] = t;
        for (size_t i = 1; i < 45; ++i) {
            t = reverse_decimal_local(t) + (seq_t)2;
            cache[i] = t;
        }
        init = true;
    }

    /* Use modulo 45 to map any n >= 1 into the cycle. */
    size_t k = (size_t)((n - (idx_t)1) % (idx_t)45);
    return cache[k];
}
