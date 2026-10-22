/*
 * A000790  Primary pretenders: least composite c such that n^c == n (mod c).
 * https://oeis.org/A000790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   least composite c such that n^c ≡ n (mod c)
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small, modpow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/modpow.h"

seq_t a(idx_t n) {
    for (seq_t c = 4; c < 1000000; c++) {
        if (is_prime_small(c)) continue;
        
        idx_t c_u64 = (idx_t)c;
        idx_t n_mod = n % c_u64;
        seq_t nc_mod = modpow(n, c_u64, c);
        
        if ((idx_t)nc_mod == n_mod) {
            return c;
        }
    }
    return 0;
}
