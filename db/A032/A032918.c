/*
 * A032918  Numbers whose set of base-11 digits is {1,3}.
 * https://oeis.org/A032918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 62, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   seqlen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose set of base‑11 digits is {1,3}.
 * a(n) is the nth smallest positive integer whose base‑11 representation
 * uses only the digits 1 and 3 (each digit is either 1 or 3).
 * The function enumerates the numbers by length and by binary pattern. */
#include <stdint.h>
#include "../utils/seqlen.h"

/* Compute the nth term (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined, but safe return */

    idx_t len = seqlen(n);                 /* number of base‑11 digits */

    /* Numbers of length 1 come first, then length 2, etc.
       Let S(L-1) = 2^L – 2 be the count of all numbers of shorter length.
       The offset within the current length is:
             offset = n – (2^len – 2) – 1  (zero‑based) */
    idx_t pre = ((idx_t)1 << len) - 2;     /* S(len-1) */
    idx_t off = n - pre - 1;               /* 0‑based index within length len */

    seq_t val = 0;
    /* Construct the number by processing the bits of off.
       Bit 0 → digit 1, Bit 1 → digit 3. Process from most significant
       to least to get the correct base‑11 value. */
    for (int i = (int)len - 1; i >= 0; --i) {
        int bit = (off >> i) & 1;
        val = val * (seq_t)11 + (seq_t)(bit ? 3 : 1);
    }
    return val;
}
