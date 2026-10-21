/*
 * A056089  Numbers k such that k^18 == 1 (mod 19^3).
 * https://oeis.org/A056089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 171, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A056089 – Numbers k such that k^18 ≡ 1 (mod 19^3).
    Formula: let R = { r | 1 ≤ r < 19^3, r^18 ≡ 1 (mod 19^3) },
            sorted ascending r1 < … < r18.
            Let M = 19^3 = 6859.
            Then a(n) = r_{ ((n-1) mod 18)+1 } + M * floor((n-1)/18).  */

#include <stdint.h>

/* local helper: modular exponentiation (uint64_t safe modulus) */
static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    uint64_t b   = base % mod;
    while (exp) {
        if (exp & 1U)
            res = (res * b) % mod;
        b = (b * b) % mod;
        exp >>= 1U;
    }
    return res;
}

/* The 18 residues modulo 6859 that satisfy k^18 ≡ 1.  */
static seq_t residues[18];
static int residues_ready = 0;

/* Compute residues once, sort ascending. */
static void init_residues(void)
{
    if (residues_ready)
        return;
    const uint64_t MOD = 6859;
    int cnt = 0;
    for (uint64_t i = 1; i <= MOD; ++i) {
        if (pow_mod_uint64(i, 18, MOD) == 1ULL) {
            residues[cnt++] = (seq_t)i;   /* fits in seq_t */
            if (cnt == 18) /* all found */
                break;
        }
    }
    /* simple selection sort: 18 elements only */
    for (int i = 0; i < 18; ++i) {
        for (int j = i + 1; j < 18; ++j) {
            if (residues[j] < residues[i]) {
                seq_t tmp = residues[i];
                residues[i] = residues[j];
                residues[j] = tmp;
            }
        }
    }
    residues_ready = 1;
}

seq_t a(idx_t n)
{
    init_residues();   /* ensure residues are known */
    const seq_t MOD_VAL = (seq_t)6859;
    idx_t idx = n - 1;            /* zero‑based index */
    idx_t t   = idx / 18;         /* block number */
    int  j    = idx % 18;         /* position within block */
    return residues[j] + (seq_t)t * MOD_VAL;
}
