/*
 * A046144  Number of primitive roots modulo n.
 * https://oeis.org/A046144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(φ(n)) if n is 1, 2, 4, p^k or 2p^k (p odd prime),
 *            otherwise a(n)=0. (By convention a(1)=1.)
 * Range:     u8 n <= 556, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/factor_u64.h"

static bool has_primitive_root(idx_t n)
{
    if (n == 1) return true;            /* convention for a(1) */
    if (n == 2 || n == 4) return true;  /* 2 and 4 admit primitive roots */

    if ((n & 1ULL) == 0) {              /* even n > 4 */
        idx_t m = n >> 1;               /* n = 2·m with m odd */
        if ((m & 1ULL) == 0) return false;
        if (m <= 1) return false;
        /* factor m, must be a single odd prime power */
        uint64_t pr[64], ex[64];
        size_t cnt;
        factor_u64((uint64_t)m, pr, ex, &cnt);
        return cnt == 1;
    } else {                            /* odd n > 1 */
        uint64_t pr[64], ex[64];
        size_t cnt;
        factor_u64((uint64_t)n, pr, ex, &cnt);
        return cnt == 1;
    }
}

seq_t a(idx_t n)
{
    if (!has_primitive_root(n)) return 0;
    if (n == 1) return 1;
    seq_t phi_n   = phi(n);             /* φ(n)   */
    seq_t phi_phi = phi((idx_t)phi_n);  /* φ(φ(n)) */
    return phi_phi;
}
