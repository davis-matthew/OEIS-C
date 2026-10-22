/*
 * A007457  Number of (j,k): j+k=n, (j,n)=(k,n)=1, j,k squarefree.
 * https://oeis.org/A007457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{i=1..n-1} mu(i*(n-i))^2
 * Range:     u8 n <= 772, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


static bool is_squarefree_u64(uint64_t n) {
    if (n <= 1) return n == 1;
    
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return false;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t i = 1; i < n; i++) {
        uint64_t product = i * (n - i);
        if (is_squarefree_u64(product)) {
            count++;
        }
    }
    return count;
}
