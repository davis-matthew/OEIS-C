/*
 * A067715  Numbers k such that the sum of exponents in the prime
 *          factorization of k is < log(k).
 * https://oeis.org/A067715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 226, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067715: a(n) = the n‑th integer k such that sum of
   prime exponents of k (Ω(k)) < ln(k).  Uses the identity
   Ω(k) < ln(k) ⇔ k > ⌊e^Ω(k)⌋, which is checked with a small
   precomputed table of floor(e^s). */

#include <stdint.h>
#include <stdbool.h>

static const uint64_t threshold[] = {
    1ULL,     /* floor(e^0) */
    2ULL,     /* floor(e^1) */
    7ULL,     /* floor(e^2) */
    20ULL,    /* floor(e^3) */
    54ULL,    /* floor(e^4) */
    148ULL,   /* floor(e^5) */
    403ULL,   /* floor(e^6) */
    1096ULL,  /* floor(e^7) */
    2980ULL,  /* floor(e^8) */
    8103ULL,  /* floor(e^9) */
    22026ULL, /* floor(e^10) */
    59874ULL, /* floor(e^11) */
    162754ULL,/* floor(e^12) */
    442413ULL,/* floor(e^13) */
    1202604ULL,/* floor(e^14) */
    3269017ULL,/* floor(e^15) */
    8886110ULL,/* floor(e^16) */
    24154952ULL,/* floor(e^17) */
    65659969ULL,/* floor(e^18) */
    178482300ULL,/* floor(e^19) */
    488165888ULL,/* floor(e^20) */
    1319261358ULL,/* floor(e^21) */
    3579109117ULL,/* floor(e^22) */
    9243646446ULL /* floor(e^23) */
};

seq_t a(idx_t n) {
    if (n == 0) return 0;
    uint64_t target = n;
    uint64_t count = 0;
    uint64_t k = 2;                     /* start checking from 2 */
    for (;;) {
        /* compute Ω(k) */
        int sum = 0;
        uint64_t t = k;
        for (uint64_t p = 2; p * p <= t; ++p) {
            while (t % p == 0) {
                ++sum;
                t /= p;
            }
        }
        if (t > 1) ++sum;               /* remaining prime factor */

        /* ensure threshold array is large enough */
        uint64_t th = (sum < sizeof(threshold)/sizeof(threshold[0])) ?
                      threshold[sum] : UINT64_MAX;

        if (k > th) {
            ++count;
            if (count == target) return (seq_t)k;
        }
        ++k;
    }
}
