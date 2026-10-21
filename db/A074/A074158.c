/*
 * A074158  Smallest multiple of n which begins with the concatenation of
 *          first n natural numbers.
 * https://oeis.org/A074158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest positive multiple of n that begins with the
 *            decimal concatenation of 1,2,…,n. (OEIS A074158)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Count decimal digits of an unsigned integer */
static inline int digit_len_idx(idx_t x)
{
    int d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

seq_t a(idx_t n)
{
    /* Pre‑allocate power tables up to a reasonable number of digits.
       For any reasonable n (≤ 10⁶) the number of digits in n is < 7,
       but we allow up to 20 for safety on arbitrary‑precision types. */
    const int MAX_DIG = 20;
    seq_t pow10_seq[MAX_DIG + 1];
    uint64_t pow10_mod[MAX_DIG + 1];

    pow10_seq[0] = 1;
    pow10_mod[0] = 1;   /* 10⁰ ≡ 1 (mod n) */
    for (int i = 1; i <= MAX_DIG; ++i)
    {
        pow10_seq[i] = pow10_seq[i - 1] * (seq_t)10;
        pow10_mod[i] = (pow10_mod[i - 1] * 10ULL) % (uint64_t)n;
    }

    /* Compute the concatenation P = 1‖2‖…‖n and its residue modulo n. */
    seq_t P = 0;
    uint64_t P_mod = 0;          /* holds P (mod n) during construction */
    for (idx_t i = 1; i <= n; ++i)
    {
        int d = digit_len_idx(i);            /* number of decimal digits of i */
        P      = P * pow10_seq[d] + (seq_t)i;          /* exact big integer */
        P_mod  = (P_mod * pow10_mod[d] + (uint64_t)i) % (uint64_t)n;
    }

    /* Search for the smallest exponent t such that
       (P * 10ᵗ + x) is a multiple of n with 0 ≤ x < 10ᵗ. */
    uint64_t pow10_mod_t = 1 % (uint64_t)n;  /* 10ᵗ (mod n), initially 1 */
    seq_t pow10_big = 1;                     /* 10ᵗ as a big integer, initially 1 */

    for (;;)
    {
        uint64_t r   = (P_mod * pow10_mod_t) % (uint64_t)n;
        uint64_t need = (uint64_t)n - r;   /* (n - r)  mod n */
        if (need == (uint64_t)n) need = 0; /* when r == 0 */

        if ((seq_t)need < pow10_big)      /* a solution is found */
        {
            return P * pow10_big + (seq_t)need;
        }

        /* increase t : multiply by 10 */
        pow10_mod_t = (pow10_mod_t * 10ULL) % (uint64_t)n;
        pow10_big   = pow10_big * (seq_t)10;
    }
}
