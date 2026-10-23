/*
 * A179418  a(n) = Number of quadratic residues of 65537 in range
 *          [(n^2)+1,(n+1)^2].
 * https://oeis.org/A179418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of quadratic residues of 65537 in range
 *            [(n^2)+1,(n+1)^2]
 * Range:     u8 n <= 239, u16 n <= 255, u32 n <= 255, u64 n <= 255,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define RES_NUM 65537
#define RES_BITS ((RES_NUM + 7) / 8)   // 8193 bytes

/* Count of quadratic residues of 65537 in [n^2 + 1, (n+1)^2] */
seq_t a(idx_t n) {
    /* Static table of quadratic residues modulo 65537. */
    static unsigned char residues[RES_BITS] = {0};
    static int initialized = 0;
    if (!initialized) {
        /* Fill table: r = i^2 mod 65537 for i = 0..65536 */
        for (idx_t i = 0; i < RES_NUM; ++i) {
            idx_t r = (i * i) % RES_NUM;
            residues[r >> 3] |= 1u << (r & 7);
        }
        initialized = 1;
    }

    idx_t N = n;                           /* n as idx_t (uint64_t) */
    idx_t start = N * N + 1;               /* n^2 + 1 */
    idx_t end   = (N + 1) * (N + 1);       /* (n+1)^2 */

    /* Residue of start modulo 65537.  Use 32‑bit for remainder. */
    uint32_t rem = (uint32_t)(start % RES_NUM);
    uint32_t count = 0;

    for (idx_t val = start; val <= end; ++val) {
        if (residues[rem >> 3] & (1u << (rem & 7))) {
            ++count;
        }
        /* Increment remainder modulo 65537. */
        ++rem;
        if (rem == RES_NUM) rem = 0;
    }

    return (seq_t)count;
}
