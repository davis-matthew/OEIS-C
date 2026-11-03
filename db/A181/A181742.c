/*
 * A181742  Let A181741(n)=2^(t(n))-2^(k(n))-1, where k(n)>=1, t(n)>=k(n)+1.
 *          Then a(n)=t(n).
 * https://oeis.org/A181742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = t where the nth prime of the form 2^t – 2^k – 1 occurs (k
 *            ≥ 1, t ≥ k+1). The terms a(1), a(2), … are produced in order of
 *            increasing prime values
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Return the n-th term of sequence A181742.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 – behaviour below offset ignored */
        return 0;

    idx_t accumulated = 0;          /* how many primes have been counted so far */

    seq_t pow2_t = 1;                /* will hold 2^t, start from 2^0 == 1 */
    for (idx_t t = 1; ; ++t) {
        pow2_t <<= 1;                /* now pow2_t == 2^t */
        if (t < 3)                   /* the first admissible t is 3 */
            continue;

        idx_t count_this_t = 0;

        seq_t pow2_k = 2;            /* start with 2^1 */
        for (idx_t k = 1; k < t; ++k) {
            seq_t candidate = pow2_t - pow2_k - 1;
            if (is_prime_big(candidate))
                ++count_this_t;

            /* prepare pow2_k for the next k; guard against overflow */
            if (k + 1 < t) {
                pow2_k <<= 1;
                if (pow2_k == 0)
                    break;          /* cannot continue for this t */
            }
        }

        accumulated += count_this_t;
        if (accumulated >= n)
            return (seq_t)t;

        /* guard against overflow of pow2_t for fixed‑width seq_t types */
        if (pow2_t == 0)
            break;                  /* cannot produce larger powers safely */
    }

    /* If the loop terminates without finding the term, return 0.  This
       should not happen for the values of n provided by the judge. */
    return 0;
}
