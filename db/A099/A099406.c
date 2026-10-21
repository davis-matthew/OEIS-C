/*
 * A099406  Decimal part of 1/a(n) starts with the n-th prime (leading zeros
 *          excluded).
 * https://oeis.org/A099406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A099406:
 *  a(n) = the smallest positive integer a such that the decimal part of
 *  1/a, with leading zeros dropped, starts with the n-th prime.
 *
 *  This is equivalent to a(n) = A052039(prime(n)), where
 *  A052039(m) = smallest a with the property above.
 *
 *  The algorithm considers, for each possible position s of the
 *  first non‑zero decimal digit, the range of a that yields the required
 *  leading digits.  The first s for which a valid range exists gives the
 *  minimal a.  All calculations are performed with the generic integer
 *  type seq_t, using only integer arithmetic.
 *
 *  Complexity:  O(number of decimal digits of prime(n)) per call,
 *  which is negligible for n up to 10,000.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Count the number of decimal digits of a 32‑bit integer. */
static int digit_count_uint32(uint32_t x) {
    int d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

seq_t a(idx_t n) {
    /* Prime corresponding to index n. */
    uint32_t p_uint = get_prime(n);
    seq_t p = (seq_t)p_uint;

    /* Number of decimal digits of the prime. */
    int d = digit_count_uint32(p_uint);

    /* 10^d as seq_t. */
    seq_t pow10_d = 1;
    for (int i = 0; i < d; ++i) {
        pow10_d *= 10;
    }

    /* 10^(s-1) will be built incrementally. */
    seq_t pow10_s_minus1 = 1;   /* for s = 1 */
    seq_t pow10_s = 10;          /* 10^s for s = 1 */

    for (idx_t s = 1; ; ++s, pow10_s_minus1 = pow10_s, pow10_s *= 10) {
        /* M = 10^(s-1+d) = pow10_s_minus1 * 10^d */
        seq_t M = pow10_s_minus1 * pow10_d;

        /* Compute lower = floor(M/(p+1)) + 1, upper = floor(M/p) */
        seq_t lower = M / (p + 1) + 1;
        seq_t upper = M / p;

        /* Intersection with the interval (10^(s-1), 10^s] */
        seq_t lower_inter = lower;
        if (lower_inter <= pow10_s_minus1) {
            lower_inter = pow10_s_minus1 + 1;
        }
        seq_t upper_inter = upper;
        if (upper_inter > pow10_s) {
            upper_inter = pow10_s;
        }

        if (lower_inter <= upper_inter) {
            /* The minimal a in this intersection is the answer. */
            return lower_inter;
        }
        /* Next iteration automatically increases s by 1. */
    }
}
