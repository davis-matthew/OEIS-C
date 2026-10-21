/*
 * A056322  Number of primitive (aperiodic) reversible strings with n beads
 *          using exactly six different colors.
 * https://oeis.org/A056322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) · A056313(n/d) where A056313(k) = (T(k) +
 *            P(k))/2 with T(k) = Σ_{i=0}^6 (-1)^i · C(6,i) · (6-i)^k P(k) =
 *            Σ_{i=0}^6 (-1)^i · C(6,i) · (6-i)^⌈k/2⌉ (palindromic strings)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   mu_of, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mu_of.h"
#include "../utils/pow_seq.h"

/* binomial coefficients C(6,i) */
static const unsigned int binom[7] = {1,6,15,20,15,6,1};

/* ---- compute A056313(k): reversible strings of length k using all 6 colors ---- */
static seq_t A056313(idx_t k)
{
    /* T(k): all strings length k using all colors at least once */
    seq_t T = 0;
    for (unsigned int i = 0; i <= 6; ++i) {
        seq_t pw = pow_seq((seq_t)(6 - i), (int)k);
        seq_t term = binom[i] * pw;
        if ((i & 1) == 0)
            T += term;
        else
            T -= term;
    }

    /* P(k): palindromic strings of length k using all colors at least once */
    idx_t half = (k + 1) / 2;       /* ceil(k/2) = number of independent positions in a palindrome */
    seq_t P = 0;
    for (unsigned int i = 0; i <= 6; ++i) {
        seq_t pw = pow_seq((seq_t)(6 - i), (int)half);
        seq_t term = binom[i] * pw;
        if ((i & 1) == 0)
            P += term;
        else
            P -= term;
    }

    /* A056313(k) = (T + P)/2 */
    return (T + P) / ((seq_t)2);
}

/* ---- public function ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t pos = 0, neg = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        idx_t q1 = n / d;
        int mu1 = mu_of(d);
        if (mu1 == 1) {
            seq_t val = A056313(q1);
            pos += val;
        } else if (mu1 == -1) {
            seq_t val = A056313(q1);
            neg += val;
        }

        idx_t d2 = n / d;
        if (d2 != d) {
            int mu2 = mu_of(d2);
            if (mu2 == 1) {
                seq_t val = A056313(n / d2);
                pos += val;
            } else if (mu2 == -1) {
                seq_t val = A056313(n / d2);
                neg += val;
            }
        }
    }

    /* pos >= neg for primitive counts */
    return pos - neg;
}
