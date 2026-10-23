/*
 * A184290  Number of distinct n X 3 toroidal 0..5 arrays.
 * https://oeis.org/A184290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(3n) * Σ_{d|n} φ(n/d) * [ 6^{3d} + 2·6^{d·gcd(n/d,3)} ]
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where φ is Euler's totient function */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

typedef uint64_t idx_t;   /* index type is uint64_t */
#define seq_from_int(x) ((seq_t)(x))   /* helper to convert integer to seq_t */

static bool phi_ready = false;
static uint32_t phi[10001];   /* preallocate for n ≤ 10000 */

/* initialise totients up to MAX_N */
static void init_phi(void) {
    if (phi_ready) return;
    const idx_t MAX_N = 10000;
    for (idx_t i = 0; i <= MAX_N; ++i) phi[i] = (uint32_t)i;
    phi[0] = 0;  /* unused */
    for (idx_t i = 2; i <= MAX_N; ++i) {
        if (phi[i] == (uint32_t)i) {            /* i is prime */
            for (idx_t j = i; j <= MAX_N; j += i)
                phi[j] -= phi[j] / (uint32_t)i;
        }
    }
    phi_ready = true;
}

/* gcd for idx_t, uses built‑in Euclidean algorithm */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) { idx_t t = a % b; a = b; b = t; }
    return a;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */

    init_phi();                    /* ensure phi[] computed at first call */

    seq_t sum = 0;                 /* accumulator for numerator */

    /* Enumerate all divisors d of n */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;

        /* divisor d1 = i */
        idx_t d1 = i;
        idx_t idx1 = n / d1;          /* n/d */
        seq_t term1 = big_pow(seq_from_int(6), seq_from_int(3 * d1));
        idx_t g1 = gcd_idx(idx1, 3);
        seq_t term2 = big_pow(seq_from_int(6), seq_from_int(d1 * g1));
        term2 *= seq_from_int(2);    /* 2 * 6^{...} */
        term1 += term2;
        sum += term1 * seq_from_int(phi[idx1]);

        /* divisor d2 = n/i, skip if same as d1 */
        if (i != n / i) {
            idx_t d2 = n / i;
            idx_t idx2 = n / d2;      /* n/d, equals i */
            seq_t term1b = big_pow(seq_from_int(6), seq_from_int(3 * d2));
            idx_t g2 = gcd_idx(idx2, 3);
            seq_t term2b = big_pow(seq_from_int(6), seq_from_int(d2 * g2));
            term2b *= seq_from_int(2);
            term1b += term2b;
            sum += term1b * seq_from_int(phi[idx2]);
        }
    }

    /* denominator is 3n */
    seq_t denom = seq_from_int(3 * n);
    return sum / denom;
}
