/*
 * A059175  For a rational number p/q let f(p/q) = p*q divided by the sum of
 *          digits of p and q; a(n) is obtained by iterating f, starting at
 *          n/1, until an integer is reached, or if no integer is ever reached
 *          then a(n) = 0.
 * https://oeis.org/A059175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 36, u64 n <= 121,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS A059175.
 *  The sequence is defined by iterating
 *      f(p/q) = (p*q) / (sumDigits(p)+sumDigits(q))
 *  starting from n/1.  The process stops when the fraction becomes an
 *  integer; that integer is returned.  If the iteration never reaches an
 *  integer (i.e. the fraction repeats), 0 is returned.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"   /* provides digit_sum(seq_t) */

/* a small helper:  gcd for unsigned __int128 */
static inline unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != 0) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/*  fraction type */
typedef struct { seq_t p; seq_t q; } frac_t;

static const int MAX_STEPS = 20000;       /* generous upper bound */

/*  main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    seq_t p = (seq_t)n;
    seq_t q = 1;

    /*  store seen fractions to detect cycles  */
    frac_t seen[MAX_STEPS];
    int seen_len = 0;
    seen[seen_len++] = (frac_t){p, q};

    for (int step = 0; step < MAX_STEPS; ++step) {
        /*  sum of digits of numerator and denominator  */
        seq_t sd_p = digit_sum(p);
        seq_t sd_q = digit_sum(q);
        seq_t s    = sd_p + sd_q;      /* sum of digits after which we divide */

        /*  compute product p*q using 128‑bit to avoid overflow                  */
        unsigned __int128 prod = (unsigned __int128)p * (unsigned __int128)q;

        /*  reduce by gcd(prod, s)                                               */
        unsigned __int128 g128 = gcd128(prod, (unsigned __int128)s);
        seq_t new_p = (seq_t)(prod / g128);
        seq_t new_q = (seq_t)(s / g128);

        /*  if denominator is 1, we have reached an integer   */
        if (new_q == 1) {
            return new_p;
        }

        /*  cycle detection  */
        bool cycle = false;
        for (int i = 0; i < seen_len; ++i) {
            if (seen[i].p == new_p && seen[i].q == new_q) {
                cycle = true;
                break;
            }
        }
        if (cycle) {
            return 0;
        }

        /*  record new state for next iteration  */
        if (seen_len < MAX_STEPS) {
            seen[seen_len++] = (frac_t){new_p, new_q};
        } else {
            /* exceeded storage, assume non‑terminating */
            return 0;
        }

        p = new_p;
        q = new_q;
    }

    /*  exceeded MAX_STEPS – treat as non‑terminating  */
    return 0;
}
