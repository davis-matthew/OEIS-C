/*
 * A056086  Numbers k such that k^12 == 1 (mod 13^3).
 * https://oeis.org/A056086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 358, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of k such that k^12 ≡ 1 (mod 13^3).  The first terms are
   1, 239, 418, 657, … as given in A056086.  The sequence is obtained
   by repeating the 12 solutions of the congruence inside successive
   blocks of size 2197 (13^3). */

#include <stdint.h>
#include <stdbool.h>

/* Modular exponentiation: computes (base^exp) mod mod, all values
   kept in type seq_t.  The routine is self‑contained and does not
   depend on external libraries. */
static seq_t modpow(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = (seq_t)1 % mod;
    base %= mod;
    while (exp) {
        if (exp & (uint64_t)1)
            result = (seq_t)((seq_t)result * base % mod);
        base = (seq_t)((seq_t)base * base % mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute the 12 residues only once. */
    static bool ready = false;
    static seq_t residues[12];
    if (!ready) {
        const seq_t MOD = (seq_t)2197;          /* 13^3 */
        int count = 0;
        for (seq_t k = (seq_t)1; k < MOD; ++k) {
            /* Skip multiples of 13, which can never satisfy k^12 ≡ 1. */
            if (k % (seq_t)13 == (seq_t)0) continue;
            if (modpow(k, (uint64_t)12, MOD) == (seq_t)1) {
                residues[count++] = k;
            }
        }
        /* The congruence has exactly 12 solutions in 1 … 2196. */
        (void)count;   /* silence unused variable warnings if checked. */
        ready = true;
    }

    /* Translate the 1‑based index n into a position in the repeated
       pattern of residues. */
    idx_t zero_based = n - (idx_t)1;          /* 0‑based index */
    seq_t block  = zero_based / (idx_t)12;    /* how many full 2197‑blocks */
    seq_t elem   = residues[zero_based % (idx_t)12];
    const seq_t MOD = (seq_t)2197;
    return elem + MOD * block;
}
