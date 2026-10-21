/*
 * A054713  Number of powers of 13 modulo n.
 * https://oeis.org/A054713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = |{13^k mod n : k ≥ 0}| (number of distinct residues of
 *            powers of 13 modulo n)
 * Range:     u8 n <= 280, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n=0, but guard the array allocation */
    size_t sz = (size_t)n;          /* n is uint64_t, but for n ≤ 10000 this fits */
    bool *seen = (bool *)calloc(sz, sizeof(bool));
    if (!seen) return 0;     /* allocation failed; return 0 as a safe fallback */

    uint64_t rem = 1 % (uint64_t)n;   /* current residue, starts with 13^0 = 1 */
    unsigned long long cnt = 0;      /* count fits in sizeof(unsigned long long) for n ≤ 10000 */

    while (!seen[rem]) {
        seen[rem] = true;
        cnt++;
        /* multiply by 13 modulo n, using 128‑bit to avoid overflow */
        rem = (uint64_t)(((__int128)rem * 13) % (uint64_t)n);
    }

    free(seen);
    return (seq_t)cnt;
}
