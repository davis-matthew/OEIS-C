/*
 * A205789  Least positive integer k such that n divides k^4-j^4 for some j in
 *          [1,k-1].
 * https://oeis.org/A205789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least positive integer k such that n divides k^4 -
 *            j^4 for some j in [1, k-1]
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Safe modular multiplication for 64‑bit values using 128‑bit arithmetic. */
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* Least k such that n | k^4 - j^4 for some 1 ≤ j < k. */
seq_t a(idx_t n)
{
    /* Handle the special case n = 1 quickly. */
    if (n == 1)
        return (seq_t)2;
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0 – return 0 as a sentinel */

    uint64_t mod_n = (uint64_t)n;

    /* Array marking which 4th‑power residues mod n have already appeared. */
    uint8_t *seen = (uint8_t*)calloc(mod_n, sizeof(uint8_t));
    if (!seen)
        return (seq_t)0;   /* memory failure – unlikely for small n */

    seq_t result = 0;
    for (uint64_t m = 1; ; ++m) {
        /* Compute r = m^4 mod n using safe modular multiplication. */
        uint64_t r = mod_mul64(m, m, mod_n);  /* m^2 mod n */
        r = mod_mul64(r, r, mod_n);          /* (m^2)^2 = m^4 mod n */

        /* First m with a repeated residue gives the answer. */
        if (seen[(size_t)r]) {
            result = (seq_t)m;
            break;
        }
        seen[(size_t)r] = 1;
    }

    free(seen);
    return result;
}
