/*
 * A066604  a(n) = 6^n mod n.
 * https://oeis.org/A066604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6^n mod n
 * Range:     u8 n <= 271, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* only for bool usage if needed */

seq_t a(idx_t n)
{
    /* a(1) is the first term; for n = 0 we return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    /* Use 64‑bit arithmetic with 128‑bit temporaries to avoid overflow */
    uint64_t mod  = n;
    uint64_t base = 6 % mod;
    uint64_t exp  = n;
    uint64_t res  = 1 % mod;

    while (exp > 0) {
        if (exp & 1)
            res = (unsigned __int128)res * base % mod;

        exp >>= 1;
        if (exp)
            base = (unsigned __int128)base * base % mod;
    }

    return (seq_t)res;
}
