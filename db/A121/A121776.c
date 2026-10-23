/*
 * A121776  Antidiagonal sums of triangle A121775.
 * https://oeis.org/A121776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d=1}^{n} φ(n/d)·C(d, n mod d) + Σ_{d=1}^{⌊n/2⌋} [n mod
 *            d == 0] · φ(n/d – 1) (for n>0), a(0)=1
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ensure_phi up to n, returning pointer to array phi[0..n] */
static seq_t *phi_cache = NULL;
static size_t phi_cache_size = 0;

static void ensure_phi(idx_t n) {
    if (phi_cache_size > (size_t)n) return;
    size_t new_size = (size_t)n + 1;
    seq_t *new_phi = (seq_t *)calloc(new_size, sizeof(seq_t));
    if (!new_phi) exit(1);          /* abort on memory failure */
    for (size_t i = 0; i < new_size; ++i) new_phi[i] = (seq_t)i;
    for (size_t i = 2; i < new_size; ++i) {
        if (new_phi[i] == (seq_t)i) {          /* i is prime */
            for (size_t j = i; j < new_size; j += i) {
                new_phi[j] -= new_phi[j] / (seq_t)i;
            }
        }
    }
    free(phi_cache);
    phi_cache = new_phi;
    phi_cache_size = new_size;
}

/* binomial coefficient C(n,k) using multiplicative formula with 128‑bit temp */
static seq_t binom_uint64(uint64_t n, uint64_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    if (k == 0) return 1;
    __int128 res = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res /= i;
    }
    return (seq_t)res;
}

extern seq_t a(idx_t n) {
    if (n == 0) return 1;
    ensure_phi(n);
    seq_t sum = 0;
    uint64_t half = n / 2;
    for (uint64_t d = 1; d <= n; ++d) {
        uint64_t q = n / d;
        uint64_t r = n % d;

        seq_t bin = binom_uint64(d, r);
        __int128 prod = (__int128)phi_cache[(size_t)q] * (__int128)bin;
        sum += (seq_t)prod;

        if (r == 0 && d <= half) {
            /* k = d term */
            sum += phi_cache[(size_t)(q - 1)];
        }
    }
    return sum;
}
