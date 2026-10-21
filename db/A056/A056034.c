/*
 * A056034  Numbers k such that k^28 == 1 (mod 29^2).
 * https://oeis.org/A056034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2180, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A056034: Numbers k such that k^28 ≡ 1 (mod 29²).
   The 28 solutions modulo 29² are the residues r₀ < r₁ < … < r₂₇.
   Every integer belonging to the sequence is one of those residues
   plus a multiple of 29²:

          a(n) = r[(n‑1) mod 28] + ((n‑1)/28) * 29²

   The array of residues is computed once on the first call by
   brute‑forcing 1 ≤ k < 29².  */
#include <stdint.h>

seq_t a(idx_t n)
{
    static int initialized = 0;          /* 0: not yet, 1: done */
    static seq_t residues[28];           /* sorted residues */

    if (!initialized) {
        const uint64_t MOD = 841ULL;      /* 29² */
        uint64_t cnt = 0;
        for (uint64_t k = 1; k < MOD && cnt < 28; ++k) {
            uint64_t res = 1, base = k, exp = 28;
            while (exp) {
                if (exp & 1) res = (res * base) % MOD;
                base = (base * base) % MOD;
                exp >>= 1;
            }
            if (res == 1) {
                residues[cnt++] = (seq_t)k;
            }
        }
        initialized = 1;
    }

    /* index of the residue (0‑based) */
    idx_t idx = (n - 1) % 28;
    seq_t residue = residues[idx];

    /* number of full periods (each adds 841) */
    idx_t periods = (n - 1) / 28;
    seq_t shift = (seq_t)periods * (seq_t)841;

    return residue + shift;
}
