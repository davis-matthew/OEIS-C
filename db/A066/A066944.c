/*
 * A066944  Numbers k such that gcd(prime(k+1) + 1, prime(k) + 1) = 10.
 * https://oeis.org/A066944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_uint64, is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that gcd(prime(k+1)+1,prime(k)+1)=10. */
#include <stdint.h>
#include "../utils/is_prime64.h"
#include "../utils/gcd_uint64.h"

seq_t a(idx_t n) {
    if (n <= 0) return (seq_t)0;
    idx_t count = 0;
    uint64_t p_prev = 2;      /* prime(1)  */
    uint64_t p_curr = 3;      /* prime(2)  */
    idx_t    k      = 1;      /* index of prime(k)  */

    while (1) {
        if (gcd_uint64(p_prev + 1, p_curr + 1) == 10) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        /* advance to next consecutive prime pair */
        ++k;
        p_prev = p_curr;
        uint64_t cand = p_curr + 2;          /* skip even numbers */
        while (!is_prime64(cand)) cand += 2;
        p_curr = cand;
    }
}
