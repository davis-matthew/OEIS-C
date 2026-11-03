/*
 * A180060  a(n) = 2^(2^n mod n) mod n.
 * https://oeis.org/A180060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2^n mod n) mod n
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 96 terms through n = 968 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    seq_t mod = (seq_t)n;                       /* modulus n as seq_t   */
    seq_t e   = powmod((seq_t)2, (uint64_t)n, mod); /* e = 2^n mod n  */
    seq_t res = powmod((seq_t)2, (uint64_t)e, mod); /* 2^e mod n       */
    return res;
}
