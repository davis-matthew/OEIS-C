/*
 * A219839  a(n) is the number of odd integers in 2..(n-1) that have a common
 *          factor (other than 1) with n.
 * https://oeis.org/A219839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊n/2⌋ − φ(2n)/2 for n ≥ 2; a(1) = 0 Where φ is Euler's
 *            totient and φ(2n) is computed via the multiplicative formula
 * Range:     u8 n <= 939, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Return the totient of m (m ≤ 2·2^63) */
static uint64_t totient_uint64(uint64_t m)
{
    uint64_t phi = m;
    uint64_t tmp = m;

    /* handle factor 2 */
    if ((tmp & 1ULL) == 0) {
        phi = phi / 2 * 1;          /* φ = φ / 2 * (2-1) */
        while ((tmp & 1ULL) == 0)
            tmp >>= 1;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            phi = phi / p * (p - 1);
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1)       /* remaining prime factor */
        phi = phi / tmp * (tmp - 1);

    return phi;
}

/* OEIS sequence A219839 */
seq_t a(idx_t n)
{
    /* a(1) = 0 as per the definition (offset 1) */
    if (n == 1)
        return 0;

    /* floor(n/2) as seq_t */
    seq_t half = ((seq_t)n) / 2;

    /* compute φ(2n) */
    uint64_t prod = (uint64_t)n;
    /* multiply by 2 with 128‑bit safety, but n ≤ 2^63 for this test set */
    prod = prod * 2U;
    uint64_t phi2n = totient_uint64(prod);

    /* φ(2n)/2 as seq_t */
    seq_t phi_half = (seq_t)(phi2n / 2ULL);

    /* result */
    return half - phi_half;
}
