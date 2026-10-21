/*
 * A220780  Nonzero terms of A220779: exponent of highest power of 2 dividing
 *          an even sum 1^n + 2^n + ... + n^n.
 * https://oeis.org/A220780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The 2-adic valuation of the even sum S(n)=Σ_{k=1}^n k^n * for
 *            n≡0 (mod 4) or n≡3 (mod 4) is * * a(i) = v2(m) - 1 if m ≡ 0 (mod
 *            4) * a(i) = v2(m+1) + v2((m+1)/4) if m ≡ 3 (mod 4) * * where i
 *            is the index in the subsequence of non‑zero terms of * A220779
 *            and m is the corresponding n value that yiel...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   trailing_zeros_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/trailing_zeros_uint64.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined; return 0 for safety */

    /* Map subsequence index n to the original n (denoted m) that has a
     * non‑zero 2‑adic valuation. */
    idx_t group = (n - 1) / 2;            /* number of full (3,4) pairs before this one */
    idx_t m = group * 4 + ((n & 1) ? 3 : 4);

    /* Compute the desired exponent. */
    unsigned val;
    if ((m & 3) == 0) {                  /* m ≡ 0 (mod 4) */
        val = trailing_zeros_uint64(m) - 1;
    } else {                              /* m ≡ 3 (mod 4) */
        uint64_t mp1 = m + 1;             /* divisible by 4 */
        val = trailing_zeros_uint64(mp1) +
              trailing_zeros_uint64(mp1 >> 2);
    }

    return (seq_t)val;
}
