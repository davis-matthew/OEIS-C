/*
 * A074194  Trimorphic numbers: a(n)^3 == a(n) (mod 10^35).
 * https://oeis.org/A074194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 15,
 *            big any n
 * Helpers:   compute_trimorphic, expmod128, mulmod128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A074194 – Trimorphic numbers modulo 10^35
    a(n)  : the nth trimorphic number (offset 1)
    a(n)  = x  where  x^3 ≡ x  (mod 10^35)  and  0 ≤ x < 10^35
    The set of such residues modulo 10^35 contains exactly
    15 elements.  By the Chinese Remainder Theorem they are
    obtained from the 5 solutions modulo 2^35 and the 3
    solutions modulo 5^35.  The function builds the list once
    and returns the n‑th element (1‑based).  For n>15 it
    returns 0. */

#include <stdint.h>
#include <stddef.h>          /* size_t */
#include "../utils/compute_trimorphic.h"
#include "../utils/expmod128.h"
#include "../utils/mulmod128.h"

/*  Public sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    static unsigned __int128 vals[15];
    static size_t    num  = 0;
    static int       init = 0;

    if (!init) {
        compute_trimorphic(vals, &num);
        init = 1;
    }

    /* 1‑based offset – return 0 for out‑of‑range indices */
    if (n < 1 || (size_t)(n-1) >= num) return (seq_t)0;

    return (seq_t)vals[n-1];
}
