/*
 * A032929  Numbers whose set of base-8 digits is {1,2}.
 * https://oeis.org/A032929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑8 digits are only 1
 *            or 2 * (OEIS A032929). Indexing starts at n = 1. The function
 *            computes the value * by determining the number of base‑8 digits
 *            k needed to reach the n‑th term * and then mapping the offset
 *            within that block from binary to the digits 1/2
 * Range:     u8 n <= 14, u16 n <= 94, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_log2_u64(idx_t v) {
    idx_t r = 0;
    while (v >>= 1) r++;
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1, guard anyway */

    /* Find k such that 2^k - 1 <= n <= 2^(k+1) - 2.
       Equivalent to floor_log2(n + 1). */
    idx_t k = floor_log2_u64(n + 1);

    /* Number of terms before the k‑digit block: start_k = 2^k - 1. */
    idx_t start_k = ((idx_t)1 << k) - 1;

    /* Offset within the block: 0-based index among the 2^k k‑digit numbers. */
    idx_t offset = n - start_k;

    /* Build the base‑8 number: digit = 1 if bit==0, else 2. */
    seq_t value = 0;
    seq_t base  = 1;      /* 8^0 */

    while (k-- > 0) {
        seq_t digit = (offset & 1) ? 2 : 1;
        value += digit * base;
        base  *= 8;
        offset >>= 1;
    }
    return value;
}
