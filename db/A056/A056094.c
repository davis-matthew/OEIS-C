/*
 * A056094  Numbers k such that k^10 == 1 (mod 11^4).
 * https://oeis.org/A056094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = residue[n] + ((n-1)/10) * 14641 where
 *            residue[0]..residue[9] are the 10 solutions of k^10 ≡ 1 (mod
 *            11^4 = 14641), sorted increasingly
 * Range:     u8 n <= 1, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a static table of residues computed once.
   It works for all lower indices, in any order of calls. */

#include <stdint.h>
#include <stddef.h>

static inline uint64_t modpow10(uint64_t base, uint64_t mod)
{
    uint64_t r = 1;
    for (int i = 0; i < 10; ++i)
        r = (r * base) % mod;
    return r;
}

seq_t a(idx_t n)
{
    /* Static table of the 10 residues */
    static seq_t residues[10];
    static int residue_count = 0;

    if (residue_count == 0) {
        /* Find all solutions 1 <= k < 14641 of k^10 ≡ 1 (mod 14641) */
        const uint64_t MOD = 14641ULL;
        for (uint64_t k = 1; k < MOD; ++k) {
            if (modpow10(k, MOD) == 1) {
                residues[residue_count++] = (seq_t)k;
            }
        }
        /* Sort the residues ascending (they are small – simple bubble sort) */
        for (int i = 0; i < residue_count - 1; ++i) {
            for (int j = i + 1; j < residue_count; ++j) {
                if (residues[i] > residues[j]) {
                    seq_t tmp = residues[i];
                    residues[i] = residues[j];
                    residues[j] = tmp;
                }
            }
        }
    }

    /* Period index and offset in the residue class */
    idx_t period = (n - 1) / 10u;
    size_t idx = (size_t)((n - 1) % 10u);

    /* Compute the term: residues[idx] + period * 14641 */
    seq_t term = residues[idx] + (seq_t)period * (seq_t)14641ULL;
    return term;
}
