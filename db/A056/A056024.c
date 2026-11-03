/*
 * A056024  Numbers k such that k^10 == 1 (mod 11^2).
 * https://oeis.org/A056024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 5415, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9891 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A056024 – Numbers k such that k^10 ≡ 1 (mod 11²)
 *
 *  a(n) = a(n‑10) + 121      for n ≥ 11
 *  a(1)…a(10) are the 10 solutions in 1…120:
 *      1, 3, 9, 27, 40, 81, 94, 112, 118, 120
 *  Thus a(n) = base[(n‑1) mod 10] + 121 * ⌊(n‑1)/10⌋
 */

/* minimal includes – idx_t and seq_t are supplied externally */
#include <stdint.h>

/* modular exponentiation – specialized for exponent 10 */
static seq_t mod_pow10(seq_t base, seq_t mod)
{
    seq_t result = (seq_t)1;
    /* multiply ten times – works for any integer seq_t, even big integers */
    for (int i = 0; i < 10; ++i) {
        result = (result * base) % mod;
    }
    return result;
}

/* compute the 10 base residues once */
static void init_base_terms(seq_t *base)
{
    seq_t mod = (seq_t)121;
    int   count = 0;

    for (idx_t k = 1; k < (idx_t)121; ++k) {
        seq_t km = (seq_t)k % mod;          /* k modulo 121 as seq_t */
        if (mod_pow10(km, mod) == (seq_t)1) {
            base[count++] = (seq_t)k;
            if (count == 10) break;
        }
    }
}

/* public function ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t base[10];
    static int   initialized = 0;

    if (!initialized) {
        init_base_terms(base);
        initialized = 1;
    }

    /* index into the base array (0‑based) */
    idx_t r = (n - 1) % 10;
    idx_t q = (n - 1) / 10;          /* number of 121‑blocks */

    /* term = base[r] + 121 * q   (all arithmetic in seq_t) */
    return base[r] + ((seq_t)q * (seq_t)121);
}
