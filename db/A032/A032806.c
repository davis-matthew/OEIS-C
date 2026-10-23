/*
 * A032806  Numbers whose set of base-6 digits is {2,3}.
 * https://oeis.org/A032806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer whose base‑6 representation
 *            consists only of the digits 2 and 3, sorted increasingly
 * Range:     u8 n <= 14, u16 n <= 126, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle an invalid index gracefully. */
    if (n == 0) return (seq_t)0;

    /* Find the minimal k such that 2^k >= n + 2. */
    uint64_t target = (uint64_t)n + 2;
    uint64_t pow    = 1;   /* 2^k */
    int       k     = 0;
    while (pow < target) {
        pow <<= 1;
        ++k;
    }

    /* Length L of the base‑6 number: L = k - 1. */
    int L = k - 1;

    /* Index inside the block of numbers of length L. */
    uint64_t offset = n - ((uint64_t)1 << L) + 1;  /* 0 <= offset < 2^L */

    /* Convert the binary representation of offset into base‑6 digits
       2/3 (0→2, 1→3). */
    seq_t value = 0;
    for (int i = 0; i < L; ++i) {
        int bit   = (offset >> (L - 1 - i)) & 1;
        seq_t d   = bit ? 3 : 2;
        value = value * 6 + d;
    }
    return value;
}
