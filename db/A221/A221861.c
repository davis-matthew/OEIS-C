/*
 * A221861  The least number k that maximizes k! mod n.
 * https://oeis.org/A221861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 280, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): least k that maximizes k! mod n (search) */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 0;

    /* modulo base (must fit in 64‑bit). */
    uint64_t mod = (uint64_t)n;

    /* current factorial value */
    seq_t fact = 1 % (seq_t)n;
    seq_t maxv = fact;
    idx_t best_k = 0;

    for (idx_t k = 1; ; ++k) {
        /* compute next factorial modulo n in 64‑bit to avoid overflow */
        uint64_t prod = (uint64_t)fact * (uint64_t)k;
        fact = (seq_t)(prod % mod);

        if (fact > maxv) {
            maxv = fact;
            best_k = k;
            if (maxv == mod - 1) break;        /* cannot be exceeded */
        }

        if (fact == 0) break;                  /* all subsequent terms are 0 */
        if (k >= n) break;                     /* safety guard; k=n gives 0 */
    }

    return (seq_t)best_k;
}
