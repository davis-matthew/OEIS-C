/*
 * A090094  a(n) is the smallest composite number coprime to n, n+1, n+2, n+3
 *          and n+4.
 * https://oeis.org/A090094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (smallest prime coprime to n, n+1, n+2, n+3, n+4)^2
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* search for the smallest prime p that does not divide any of
       n, n+1, n+2, n+3, n+4.  Then return p*p.              */
    for (uint64_t p = 2; ; ++p) {
        if (!is_prime(p))
            continue;
        bool ok = true;
        for (int i = 0; i < 5; ++i) {
            if (((n + (idx_t)i) % p) == 0) {
                ok = false;
                break;
            }
        }
        if (ok) {
            /* cast to seq_t and square */
            seq_t r = (seq_t)p * (seq_t)p;
            return r;
        }
    }
}
