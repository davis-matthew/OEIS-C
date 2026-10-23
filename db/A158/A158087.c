/*
 * A158087  Prime numbers p such that largest digit of p / smallest digit of p
 *          = prime.
 * https://oeis.org/A158087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   digit_counts, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A158087: the sequence of prime numbers p such that
   the quotient of the largest single decimal digit of p by the
   smallest digit of p is a prime  (i.e. 2, 3, 5, or 7). */

/*  The algorithm simply tests successive primes until the n‑th term
    is found.  It uses digit_counts to obtain the digit frequency
    of each candidate prime so that no dynamic allocation is
    required when examining its decimal digits. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t) */
#include "../utils/digit_counts.h"  /* void digit_counts(seq_t, unsigned char[10]) */

static inline bool test_digit_condition(seq_t p)
{
    unsigned char cnt[10] = {0};
    digit_counts(p, cnt);            /* count digits of p */

    if (cnt[0] > 0)                 /* zero digit makes the ratio undefined */
        return false;

    uint8_t min_d = 0, max_d = 0;

    for (int i = 1; i < 10; ++i)      /* find smallest non‑zero digit */
        if (cnt[i] > 0) { min_d = (uint8_t)i; break; }

    for (int i = 9; i >= 1; --i)      /* find largest digit */
        if (cnt[i] > 0) { max_d = (uint8_t)i; break; }

    if (max_d % min_d != 0)          /* ratio must be an integer */
        return false;

    uint8_t ratio = max_d / min_d;   /* 1‑digit ratio, always integer */

    return ratio == 2 || ratio == 3 || ratio == 5 || ratio == 7;
}

seq_t a(idx_t n)
{
    idx_t count     = 0;
    idx_t candidate = 2;

    for (;;) {
        if (is_prime(candidate)) {
            seq_t p = (seq_t)candidate;
            if (test_digit_condition(p)) {
                ++count;
                if (count == n)
                    return p;
            }
        }

        /* advance to the next candidate prime: after 2 only odds */
        if (candidate <= 2)
            candidate = 3;
        else
            candidate += 2;
    }
}
