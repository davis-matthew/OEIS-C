/*
 * A220023  The period with which the powers of n repeat mod 1000.
 * https://oeis.org/A220023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the eventual period of the sequence n^i (mod 1000). The
 *            function * computes the smallest positive k such that n^i mod
 *            1000 = n^(i+k) mod 1000 * for all sufficiently large i. This is
 *            the cycle length of the sequence of * powers of n modulo 1000
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Work modulo 1000.  Reduce n first to keep multiplications small. */
    const seq_t MOD = 1000ULL;
    seq_t r = (seq_t)(n % 1000ULL);

    /* Track first index each residue value was seen.  -1 means unseen. */
    int16_t seen[1000];
    for (int i = 0; i < 1000; ++i) seen[i] = -1;

    seq_t val = 1 % MOD;          /* n^0 mod 1000 */
    int idx = 0;
    seen[1] = 0;                  /* residue 1 was seen at index 0 */

    for (int i = 1; ; ++i) {
        val = (val * r) % MOD;    /* next power modulo 1000 */
        int v = (int)val;         /* value fits in int because val < 1000 */

        if (seen[v] != -1) {
            /* Cycle found: period = current index - first occurrence index. */
            return (seq_t)(i - seen[v]);
        }
        seen[v] = i;
    }
}
