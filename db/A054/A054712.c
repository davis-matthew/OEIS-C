/*
 * A054712  Number of powers of 12 modulo n.
 * https://oeis.org/A054712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = |{12^k mod n : k ≥ 0}| (number of distinct powers of 12
 *            modulo n)
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   f_mod12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/f_mod12.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined, but guard against */
    if (n == 1) return 1;           /* trivial case */

    seq_t mod = (seq_t)n;
    seq_t x0 = 1 % mod;             /* first residue (12^0 mod n) */

    /* Floyd's cycle-finding algorithm */
    seq_t tortoise = f_mod12(x0, mod);
    seq_t hare = f_mod12(f_mod12(x0, mod), mod);

    while (tortoise != hare) {
        tortoise = f_mod12(tortoise, mod);
        hare = f_mod12(f_mod12(hare, mod), mod);
    }

    /* Find start of cycle (mu) */
    seq_t mu = 0;
    tortoise = x0;
    while (tortoise != hare) {
        tortoise = f_mod12(tortoise, mod);
        hare = f_mod12(hare, mod);
        mu++;
    }

    /* Find length of cycle (lambda) */
    seq_t lam = 1;
    hare = f_mod12(tortoise, mod);
    while (tortoise != hare) {
        hare = f_mod12(hare, mod);
        lam++;
    }

    return mu + lam;   /* total distinct residues */
}
