/*
 * A023750  Plaindromes: numbers whose digits in base 8 are in nondecreasing
 *          order.
 * https://oeis.org/A023750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 1254, u32 n <= 9000, u64 n <= 9000,
 *            u128 n <= 9000, big any n
 * Validated: standard, pass, 82 terms through n = 8175 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the nth number (with a(1)=0) whose base‑8 digits are
 * non‑decreasing (from most significant to least). The implementation
 * computes the value by combinatorial ranking of weakly increasing digit
 * sequences.
 */

#include <stdint.h>

/* Compute C(n,k) for 0 <= k <= n, using 128‑bit arithmetic to avoid overflow.
   n is small (<= 250), k <= 6. Returns the exact binomial coefficient. */
static inline __uint128_t comb128(unsigned n, unsigned k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    __uint128_t r = 1;
    for (unsigned i = 1; i <= k; ++i) {
        r = r * (n - k + i);
        r /= i;
    }
    return r;
}

/* Return the binomial coefficient C(n,6) as idx_t (u64).  For our
   required n (len+6) this fits in 64 bits. */
static inline idx_t binom_idx_t(unsigned n)
{
    __uint128_t v = comb128(n, 6);
    return (idx_t)v;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(1) = 0 */
    if (n == 1) return (seq_t)0;

    /* Rank among non‑zero terms (k = 1 means the first non‑zero term). */
    idx_t k = n - 1;          /* 1‑based rank of the desired non‑zero number */

    /* Find the length of the base‑8 representation. */
    idx_t cum = 0;
    unsigned len = 0;
    while (1) {
        ++len;                               /* length >= 1 */
        idx_t cnt = binom_idx_t(len + 6);     /* C(len+6,6) */
        if (cum + cnt >= k) break;           /* target length found */
        cum += cnt;
    }

    /* 0‑based rank within sequences of this length. */
    idx_t rank = k - cum - 1;                /* 0 <= rank < C(len+6,6) */

    /* Unrank to get the actual digits. */
    unsigned prev_digit = 1;                 /* first digit cannot be 0 */
    seq_t value = 0;                         /* decimal value */

    for (unsigned pos = 0; pos < len; ++pos) {
        unsigned remaining = len - pos - 1;  /* digits left after current */
        unsigned chosen_digit = 0;

        for (unsigned d = prev_digit; d <= 7; ++d) {
            /* Count how many sequences start with digit d at this position. */
            unsigned types = 8 - d;          /* available digits >= d */
            unsigned nC = types + remaining - 1;
            unsigned kC = remaining;
            __uint128_t cnt128 = comb128(nC, kC);
            if ((__uint128_t)rank < cnt128) {
                chosen_digit = d;
                break;
            } else {
                rank -= (idx_t)cnt128;  /* safe: cnt fits in idx_t for our ranges */
            }
        }

        /* Append chosen_digit to the result. */
        value = value * (seq_t)8 + (seq_t)chosen_digit;
        prev_digit = chosen_digit;
    }

    return value;
}
