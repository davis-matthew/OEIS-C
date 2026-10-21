/*
 * A061509  Write n in decimal, omit 0's, replace the k-th digit d[k] with the
 *          k-th prime, raised to d[k]-th power and multiply.
 * https://oeis.org/A061509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, less, nonn
 * Formula:   a(n) = ∏ p_k^{d_k} where d_k are the non‑zero decimal digits of
 *            n, processed left‑to‑right (k = 1 for the most significant
 *            non‑zero digit)
 * Range:     u8 n <= 7, u16 n <= 28, u32 n <= 178, u64 n <= 4998,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Prime numbers used: enough for any 64‑bit decimal integer (max 20 non‑zero digits). */
static const uint64_t primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL,
    73ULL, 79ULL, 83ULL, 89ULL, 97ULL
};
static const size_t primes_count = sizeof(primes) / sizeof(primes[0]);

/* Local power helper for exponents 1–9. */
static inline seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t r = 1;
    while (exp) {
        r *= base;
        exp--;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* a(0) is the empty product. */
    if (n == 0)
        return (seq_t)1;

    /* Extract decimal digits (least significant first). */
    idx_t tmp = n;
    idx_t digits[20];          /* 20 digits are enough for a 64‑bit integer. */
    size_t len = 0;
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }

    seq_t result = (seq_t)1;
    size_t prime_idx = 0;

    /* Process digits from most significant to least, skipping zeros. */
    for (size_t i = 0; i < len; i++) {
        idx_t d = digits[len - 1 - i];   /* most significant digit first */
        if (d == 0)
            continue;
        if (prime_idx < primes_count) {
            result *= pow_uint((seq_t)primes[prime_idx], d);
            prime_idx++;
        }
    }

    return result;
}
