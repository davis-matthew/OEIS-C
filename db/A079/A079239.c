/*
 * A079239  Numbers n in which the last K digits of n form an integer
 *          divisible by K^3, for K = 1, 2, ..., M, where M is the number of
 *          digits in n.
 * https://oeis.org/A079239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 46, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079239 – for each integer n let M be its number of decimal
   digits.  n is a member of the sequence iff for every k = 1, 2, …, M
   the last k decimal digits of n form an integer divisible by k³.  This
   function returns the n‑th member (offset = 1).  The test is performed
   digit by digit so that it works for every possible definition of
   seq_t (small unsigned integers up to arbitrary‑precision classes). */

#include <stddef.h>

/* Return the number of decimal digits of x (x ≥ 1). */
static int num_digits(seq_t x)
{
    int d = 0;
    do {
        x /= (seq_t)10;
        ++d;
    } while (x != (seq_t)0);
    return d;
}

/* Test whether x satisfies the divisibility conditions of A079239. */
static int is_good(seq_t x)
{
    int M = num_digits(x);

    for (int k = 1; k <= M; ++k) {
        /* k³ fits in seq_t for all practical k (k ≤ M ≤ number of digits of x). */
        seq_t k3 = (seq_t)k * (seq_t)k * (seq_t)k;

        /* Compute the last k digits of x modulo k³ without forming 10^k. */
        seq_t last_mod = 0;
        seq_t mult      = 1 % k3;          /* current place value mod k³ */
        seq_t val       = x;
        int  digits     = 0;

        while (digits < k) {
            seq_t digit    = val % (seq_t)10;
            val           /= (seq_t)10;
            last_mod = (digit * mult + last_mod) % k3;
            mult      = (mult * (seq_t)10) % k3;
            ++digits;
        }

        if (last_mod != (seq_t)0)
            return 0;                      /* fails for this k */
    }
    return 1;                               /* satisfies all k */
}

/* Return the n‑th term of the sequence (offset = 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for the offset */

    idx_t count    = 0;
    seq_t candidate = (seq_t)1;

    while (1) {
        if (is_good(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
