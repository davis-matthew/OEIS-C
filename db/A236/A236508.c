/*
 * A236508  a(n) = |{0 < k < n-2: p = 2*phi(k) + phi(n-k)/2 - 1, p + 2, p + 6
 *          and prime(p) + 6 are all prime}|, where phi(.) is Euler's totient
 *          function.
 * https://oeis.org/A236508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{0 < k < n-2 : p = 2*phi(k)+phi(n-k)/2-1, p, p+2, p+6
 *            prime, and prime(p)+6 prime}
 * Range:     u8 n <= 67, u16 n <= 2820, u32 n <= 4966, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/calc_phi.h"
#include "../utils/is_prime_big.h"
#include "../utils/get_prime.h"

/* Helper: convert idx_t to seq_t and compute Euler totient. */
static seq_t totient(idx_t n) {
    return calc_phi((seq_t)n);
}

seq_t a(idx_t n)
{
    /* No k satisfies 0<k<n-2 when n ≤ 3. */
    if (n <= 3)
        return 0;

    seq_t count = 0;
    for (idx_t k = 1; k <= n - 3; ++k) {
        seq_t phi_k   = totient(k);
        seq_t phi_nk  = totient(n - k);

        /* p = 2*phi(k) + phi(n-k)/2 - 1 */
        seq_t p = 2 * phi_k + phi_nk / 2 - 1;

        if (!is_prime_big(p))
            continue;
        if (!is_prime_big(p + 2))
            continue;
        if (!is_prime_big(p + 6))
            continue;

        /* prime(p) + 6 must be prime */
        uint32_t prime_p = get_prime((idx_t)p);
        seq_t prime_p_plus6 = (seq_t)prime_p + 6;
        if (!is_prime_big(prime_p_plus6))
            continue;

        ++count;
    }
    return count;
}
