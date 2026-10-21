/*
 * A066439  a(n) = 8^n mod n.
 * https://oeis.org/A066439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8^n mod n
 * Range:     u8 n <= 271, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t mod = n;
    uint64_t res = 1 % mod;
    uint64_t base = 8 % mod;
    idx_t e = n;
    while (e) {
        if (e & 1)
            res = (unsigned __int128)res * base % mod;
        base = (unsigned __int128)base * base % mod;
        e >>= 1;
    }
    return (seq_t)res;
}
