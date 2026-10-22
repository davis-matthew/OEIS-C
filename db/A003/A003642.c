/*
 * A003642  Number of genera of imaginary quadratic field with discriminant
 *          -k, k = A191483(n).
 * https://oeis.org/A003642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(omega(A191483(n)) - 1), where A191483(n) = 4*m and m
 *            runs over the squarefree numbers with m == 1 or 2 (mod 4); found
 *            by a linear search
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool is_squarefree(uint64_t n) {
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % (p * p) == 0) return false;
    }
    return true;
}

static uint64_t get_a191483(idx_t n) {
    idx_t count = 0;
    for (uint64_t m = 1; ; m++) {
        if ((m % 4 == 1 || m % 4 == 2) && is_squarefree(m)) {
            count++;
            if (count == n) return 4 * m;
        }
    }
}

static int count_distinct_primes(uint64_t n) {
    int count = 0;
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            count++;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) count++;
    return count;
}

seq_t a(idx_t n) {
    uint64_t k = get_a191483(n);
    int omega = count_distinct_primes(k);
    seq_t r = 1;
    r <<= (omega - 1);
    return r;
}
