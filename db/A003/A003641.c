/*
 * A003641  Number of genera of imaginary quadratic field with discriminant
 *          -k, k = A039957(n).
 * https://oeis.org/A003641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(omega(A039957(n)) - 1), where A039957(n) is the n-th
 *            squarefree positive integer ≡ 3 mod 4, and omega(k) is the
 *            number of distinct prime factors of k
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static int count_distinct_prime_factors(uint64_t n, bool *is_sf) {
    int count = 0;
    *is_sf = true;
    
    for (uint64_t i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            count++;
            int exp = 0;
            while (n % i == 0) {
                n /= i;
                exp++;
            }
            if (exp > 1) *is_sf = false;
        }
    }
    if (n > 1) count++;
    
    return count;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    for (uint64_t k = 3; ; k += 4) {
        bool is_sf;
        int omega = count_distinct_prime_factors(k, &is_sf);
        if (is_sf) {
            count++;
            if (count == n) {
                return ((seq_t)1) << (omega - 1);
            }
        }
    }
}
