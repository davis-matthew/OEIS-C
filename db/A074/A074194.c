/*
 * A074194  Trimorphic numbers: a(n)^3 == a(n) (mod 10^35).
 * https://oeis.org/A074194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 15,
 *            big any n
 * Validated: full, pass, 15 terms through n = 15 (2026-11-02)
 * Helpers:   mulmod_u128
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
#include "../utils/mulmod_u128.h"

/* Compute all 15 trimorphic residues modulo 10^35 ----------------------------- */
static void compute_trimorphic(unsigned __int128 *items, size_t *cnt)
{
    /* Moduli 2^35 and 5^35 */
    const unsigned __int128 M2 = ((unsigned __int128)1) << 35;          /* 2^35      */
    unsigned __int128 M5 = 1;
    for (int i = 0; i < 35; ++i) M5 *= 5;                             /* 5^35      */
    const unsigned __int128 MOD = M2 * M5;                              /* 10^35     */

    /* Residues modulo 2^35 : 0,1,-1,2^34-1,2^34+1 */
    const unsigned __int128 R2[5] = {
        0,
        1,
        M2 - 1,
        (M2 >> 1) - 1,
        (M2 >> 1) + 1
    };

    /* Residues modulo 5^35 : 0,1,-1 */
    const unsigned __int128 R5[3] = {
        0,
        1,
        M5 - 1
    };

    /* Inverse of 2^35 modulo 5^35.
       Compute first inv2 = 2^{-1} mod 5^35 = (5^35+1)/2.
       Then invM2 = (inv2)^35 mod 5^35.            */
    unsigned __int128 inv2  = (M5 + 1) >> 1;              /* (5^35+1)/2   */
    unsigned __int128 invM2 = 1;
    for (int i = 0; i < 35; ++i) {
        invM2 = mulmod_u128(invM2, inv2, M5);
    }

    size_t k = 0;
    for (size_t i = 0; i < 5; ++i) {
        unsigned __int128 r2 = R2[i];
        for (size_t j = 0; j < 3; ++j) {
            unsigned __int128 r5 = R5[j];
            unsigned __int128 diff = (r5 >= r2) ? (r5 - r2) : (r5 + M5 - r2);
            unsigned __int128 t    = mulmod_u128(diff, invM2, M5);
            unsigned __int128 x    = r2 + M2 * t;   /* 0 ≤ x < MOD */
            items[k++] = x;
        }
    }

    /* sort ascending – small array, insertion sort OK */
    for (size_t i = 1; i < k; ++i) {
        unsigned __int128 v = items[i];
        size_t j = i;
        while (j > 0 && items[j-1] > v) {
            items[j] = items[j-1];
            --j;
        }
        items[j] = v;
    }
    *cnt = k;
}

/* expmod (base^exp  mod mod) – exp fits in unsigned int, base < mod < 2^127 */
static unsigned __int128 expmod128(unsigned __int128 base,
                                   unsigned int exp,
                                   unsigned __int128 mod)
{
    unsigned __int128 result = 1;
    while (exp) {
        if (exp & 1) result = mulmod_u128(result, base, mod);
        base = mulmod_u128(base, base, mod);
        exp >>= 1;
    }
    return result;
}

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
