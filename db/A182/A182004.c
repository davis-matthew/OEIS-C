/*
 * A182004  Expansion of q^(-1/4) * (eta(q^4) * eta(q^25) + eta(q) *
 *          eta(q^100))^2 / (eta(q^2) * eta(q^50)) in powers of q.
 * https://oeis.org/A182004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(4*n + 1), where b is the multiplicative function
 *            defined by OEIS entry A182004: * b(1) = 1 * b(p^e) = 0 for p = 2
 *            or 5 * b(p^e) = (1 + (-1)^e)/2 when p ≡ 3 (mod 4) * b(p^e) = (e
 *            + 1) * s^e with s = Kronecker(10, p) for other primes p
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers; no <math.h> or floating point usage. */
#include <stdint.h>
#include <stdbool.h>

/* Legendre symbols for 2 and 5; used to compute Kronecker(10, p). */
static inline int legendre2(uint64_t p) {
    /* (-1)^((p^2-1)/8) for odd p */
    switch (p & 7) {
        case 1: case 7: return 1;
        case 3: case 5: return -1;
        default:          return 0;   /* p==2 never reaches here */
    }
}

static inline int legendre5(uint64_t p) {
    /* Legendre symbol (p|5) = (5|p) by quadratic reciprocity. */
    uint64_t r = p % 5;
    switch (r) {
        case 1: case 4: return 1;
        case 2: case 3: return -1;
        default:          return 0;   /* p==5 never reaches here */
    }
}

static inline int kronecker10(uint64_t p) {
    /* p odd, p≠5 */
    return legendre2(p) * legendre5(p);   /* s = ±1 */
}

/* Compute the multiplicative function b(m). m fits in uint64_t. */
static seq_t compute_b(uint64_t m) {
    if (m == 1) return 1;

    uint64_t tmp = m;
    seq_t result = 1;

    /* Handle factor 2: any power gives zero. */
    if ((tmp & 1ULL) == 0) {
        return 0;
    }

    /* Handle factor 5: any power gives zero. */
    while (tmp % 5ULL == 0) {
        return 0;   /* as e>0 -> 0 */
    }

    /* Trial division for odd primes. */
    uint64_t p;
    for (p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            if (p % 4 == 3) {                 /* p ≡ 3 (mod 4) */
                if (e & 1) return 0;          /* odd exponent -> 0 */
                /* even exponent contributes factor 1; skip */
            } else {                           /* p ≡ 1 (mod 4) */
                seq_t term = (seq_t)(e + 1);
                int s = kronecker10(p);
                if (s == -1 && (e & 1)) term = -term;
                result *= term;
            }
        }
    }

    if (tmp > 1) {           /* remaining prime factor (exponent 1) */
        uint64_t q = tmp;
        if (q % 4 == 3) {
            return 0;       /* odd exponent of a 3 mod 4 prime */
        } else {           /* q ≡ 1 (mod 4) */
            seq_t term = 2; /* e+1 with e = 1 */
            int s = kronecker10(q);
            if (s == -1) term = -term;
            result *= term;
        }
    }

    return result;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    uint64_t m = (uint64_t)4 * n + 1;   /* m = 4*n + 1 */
    return compute_b(m);
}
