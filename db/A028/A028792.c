/*
 * A028792  Nonsquares mod 79.
 * https://oeis.org/A028792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028792: Nonsquares mod 79
   a(n) = the n-th positive integer k satisfying (k|79) = -1   (Legendre symbol)
   The sequence consists of one non‑residue from each block of 79 consecutive integers.
   There are 39 non‑residues per block, so
   a(n) = block·79 + r[ pos ],  with
           pos   = (n-1) % 39
           block = (n-1) / 39
           r[]  = sorted list of the 39 residue classes a (1 ≤ a < 79) with (a|79)=–1
   The list r[] is generated once at first call.  */

/* Standard headers for integer and boolean types */
#include <stdint.h>
#include <stdbool.h>

/* Helper: fast modular exponentiation for unsigned 64‑bit integers */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        exp >>= 1;
        if (exp) base = (base * base) % mod;
    }
    return result;
}

/* The sequence function required by the test harness */
seq_t a(idx_t n) {
    /* Undefined for n = 0 – return 0 as a placeholder */
    if (n == 0) return 0;

    /* One‑time initialization of the list of non‑residue residue classes modulo 79 */
    static bool initialized = false;
    static seq_t nonres[39];
    if (!initialized) {
        /* Deterministically compute the non‑residues */
        idx_t count = 0;
        for (idx_t m = 1; m < 79; ++m) {
            /* Legendre symbol (m|79) ≡ m^39 (mod 79) via Euler's criterion */
            uint64_t val = mod_pow_uint64((uint64_t)m, 39u, 79u);
            if (val == 78u) {            /* 78 ≡ –1 (mod 79) → non‑residue */
                nonres[count++] = (seq_t)m;
            }
        }
        /* There must be exactly 39 non‑residues for a prime p ≡ 3 (mod 4) */
        (void)count;   /* Silence unused warning ifASSERT disabled */
        initialized = true;
    }

    /* Compute the position within the current 79‑block and the block number */
    idx_t pos   = (n - 1) % 39u;
    idx_t block = (n - 1) / 39u;

    /* The n‑th nonsquare modulo 79 */
    return (seq_t)block * 79u + nonres[pos];
}
