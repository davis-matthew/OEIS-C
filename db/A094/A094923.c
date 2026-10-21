/*
 * A094923  (8^n-1)/7 mod n.
 * https://oeis.org/A094923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((8^n - 1)/7) mod n (OEIS A094923)
 * Range:     u8 n <= 285, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   powmod_128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/powmod_128.h"

/* External linkage as required by the test harness. */
seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset is 1, but safety */
    unsigned __int128 mod = (unsigned __int128)7 * n;  /* 7*n fits in 128 bits comfortably */
    unsigned __int128 pow = powmod_128(8, n, mod);     /* 8^n mod (7*n) */
    /* compute (8^n - 1) mod (7*n) and divide by 7 */
    unsigned __int128 tmp = (pow + mod - 1) % mod;      /* (8^n - 1) mod (7*n) */
    unsigned __int128 q   = tmp / 7;                    /* q = (8^n - 1)/7  (exact division) */
    unsigned __int128 r   = q % n;                      /* ( (8^n - 1)/7 ) mod n */
    return (seq_t)r;
}
