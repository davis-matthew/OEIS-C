/*
 * A013707  Decimal expansion of Sum_{k=1..50000} (-1)^(k+1) / k.
 * https://oeis.org/A013707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A013707
 *  Decimals of the sum of 1/k for 25001 ≤ k ≤ 50000.
 *  The series S = Σ_{k=25001}^{50000} 1/k
 *  satisfies a(n)= ⌊ S·10^{n+1} ⌋ mod 10  (n≥0).
 *
 *  The computation uses the identity
 *      S = Σ floor(10^{n+10}/k)/10^{10}  (mod 10)
 *  and keeps only the least significant 10 decimal digits of the partial
 *  sum, thus avoiding overflow for 64‑bit builds.
 */

#include <stdint.h>

/* modular multiplication a*b mod mod, using 128‑bit intermediate */
static inline uint64_t mulmod_uint64(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

/* modular exponentiation: base^exp mod mod */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    while (exp) {
        if (exp & 1) result = mulmod_uint64(result, base, mod);
        base = mulmod_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    const uint64_t START_K = 25001ULL;
    const uint64_t END_K   = 50000ULL;
    const uint64_t MOD10   = 10000000000ULL;  /* 10^10 */
    const uint64_t DIV10_9 = 1000000000ULL;   /* 10^9 */

    uint64_t P = n + 10;      /* exponent for 10^P */
    uint64_t sum_mod = 0;     /* partial sum modulo 10^10 */

    for (uint64_t k = START_K; k <= END_K; ++k) {
        uint64_t modM = k * MOD10;                     /* k·10^10 */
        uint64_t a_mod = mod_pow_uint64(10ULL, P, modM);/* 10^P mod (k·10^10) */
        uint64_t r_mod = mod_pow_uint64(10ULL, P, k);    /* 10^P mod k   */
        uint64_t q_mod = (a_mod - r_mod) / k;            /* floor(10^P/k) mod 10^10 */
        sum_mod += q_mod;
        if (sum_mod >= MOD10) sum_mod -= MOD10;          /* keep modulo 10^10 */
    }

    uint64_t digit = sum_mod / DIV10_9;  /* the nth decimal digit (0‑based) */
    return (seq_t)digit;
}
