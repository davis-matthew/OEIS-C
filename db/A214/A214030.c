/*
 * A214030  The multiplication continued fraction sequence for 2,3,2,3...
 * https://oeis.org/A214030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = z_U(n)-1 where z_U(n) = min{k≥1 : n | U(k)}
 * Range:     u8 n <= 276, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /*  For n = 1, the definition gives a(1) = 0  */
    if (n <= 1)
        return 0;

    /*  Work modulo n using 64‑bit arithmetic; n ≤ 10^4 in practice  */
    uint64_t mod   = (uint64_t)n;
    uint64_t r_prev = 1 % mod;   /*  U(1) mod n  */
    uint64_t r_cur  = 2 % mod;   /*  U(2) mod n  */

    if (r_cur == 0)       /*  n | U(2)  */
        return (seq_t)1;

    /*  Iterate through U(k) until the residue becomes 0  */
    for (idx_t k = 3; ; ++k) {
        /*  Coefficient alternates 2,3,2,3,…  */
        uint64_t coeff = ((k - 2) & 1) ? 3 : 2;
        uint64_t r_next = (coeff * r_cur + r_prev) % mod;

        if (r_next == 0)
            return (seq_t)(k - 1);   /*  a(n) = z_U(n) - 1  */

        r_prev = r_cur;
        r_cur  = r_next;
    }
}
