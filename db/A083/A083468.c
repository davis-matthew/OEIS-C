/*
 * A083468  Smallest multiple of n which is the reverse concatenation of n
 *          consecutive numbers; or 0 if no such number exists.
 * https://oeis.org/A083468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest multiple of n that equals the reverse
 *            concatenation of n consecutive integers k, k+1, …, k+n-1. The
 *            concatenation is performed in natural order, then the order of
 *            the individual blocks (numbers) is reversed. The sequence is
 *            computed by searching for the minimum starting integ...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* pre‑compute 10^d for d = 0 … 20 (enough for 64‑bit, 128‑bit or
   arbitrary precision seq_t ) */
static seq_t pow10_seq[21] = {0};

static void init_pow10_seq(void)
{
    if (pow10_seq[0] != 0) return;          /* already initialised */
    pow10_seq[0] = (seq_t)1;
    for (int d = 1; d <= 20; ++d)
        pow10_seq[d] = pow10_seq[d - 1] * (seq_t)10;
}

/* number of decimal digits of a non‑negative integer x (idx_t) */
static int digits_idx(idx_t x)
{
    int d = 0;
    do { ++d; x /= 10; } while (x);
    return d;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_pow10_seq();                     /* ensure pow10_seq ready */

    /* pre‑compute 10^d mod n for d up to 20 (should be enough) */
    idx_t pow10_mod[21];
    pow10_mod[0] = 1 % n;
    for (int d = 1; d <= 20; ++d)
        pow10_mod[d] = (pow10_mod[d - 1] * 10) % n;

    /* search for the smallest starting integer k >= 1 that makes
       the reversed concatenation divisible by n. */
    for (idx_t k = 1; ; ++k)
    {
        idx_t rem = 0;
        for (idx_t cur = k + n - 1; cur >= k; --cur)
        {
            int d = digits_idx(cur);
            rem = (rem * pow10_mod[d] + (cur % n)) % n;
            if (cur == k) break;        /* unsigned wraparound guard */
        }

        if (rem == 0)
        {
            /* construct the actual number using seq_t arithmetic */
            seq_t val = 0;
            for (idx_t cur = k + n - 1; cur >= k; --cur)
            {
                int d = digits_idx(cur);
                val = val * pow10_seq[d] + (seq_t)cur;
                if (cur == k) break;     /* unsigned wraparound guard */
            }
            return val;
        }
    }
    /* never reached */
    return 0;
}
