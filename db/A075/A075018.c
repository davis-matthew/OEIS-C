/*
 * A075018  Smallest k such that the concatenation k, k-1,k-2,k-3 is divisible
 *          by n; or 0 if no such number exists.
 * https://oeis.org/A075018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow10_mod_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A075018
   a(n) = smallest k ≥ 3 such that the decimal concatenation
          k, k-1, k-2, k-3 is divisible by n; returns 0 if none. */

#include <stdbool.h>
#include "../utils/pow10_mod_exp.h"

/* number of decimal digits of an idx_t (circularly safe because idx_t is unsigned) */
static inline int digit_count_idx(idx_t v) {
    int d = 0;
    do {
        ++d;
        v /= 10;
    } while (v);
    return d;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* safety for invalid input */

    seq_t n_mod = (seq_t)n;
    idx_t k = 3;                          /* minimal k */

    while (1) {
        /* digit lengths of k, k-1, k-2, k-3 */
        int l0 = digit_count_idx(k);
        int l1 = digit_count_idx(k - 1);
        int l2 = digit_count_idx(k - 2);
        int l3 = digit_count_idx(k - 3);

        /* pre‑compute powers of 10 modulo n for the needed lengths */
        seq_t p1 = pow10_mod_exp(l1, n_mod);  /* 10^l1 mod n */
        seq_t p2 = pow10_mod_exp(l2, n_mod);  /* 10^l2 mod n */
        seq_t p3 = pow10_mod_exp(l3, n_mod);  /* 10^l3 mod n */

        /* modular construction of the concatenation */
        seq_t r = (seq_t)k % n_mod;
        r = (r * p1) % n_mod;
        r = (r + ((seq_t)(k - 1) % n_mod)) % n_mod;
        r = (r * p2) % n_mod;
        r = (r + ((seq_t)(k - 2) % n_mod)) % n_mod;
        r = (r * p3) % n_mod;
        r = (r + ((seq_t)(k - 3) % n_mod)) % n_mod;

        if (r == 0) {
            return (seq_t)k;               /* found the smallest k */
        }

        ++k;                              /* next candidate */
    }
}
