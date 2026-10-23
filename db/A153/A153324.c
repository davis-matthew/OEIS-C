/*
 * A153324  Numbers n such that n^2 is concatenation of eight 2-digit primes.
 * https://oeis.org/A153324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, base
 * Formula:   a(n) is the n‑th integer whose square is a concatenation of
 *            eight two‑digit primes. Computed by a one‑time brute‑force
 *            precomputation
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Two‑digit primes (ordered ascending) */
static const int PRIMES2D[21] = {
    11, 13, 17, 19, 23, 29, 31, 37,
    41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97
};

/* The sequence is memoised in a static array.
 * The array is allocated on first use.  */
static seq_t *VALUES = NULL;   /* pointer to array of seq_t */
static size_t  LEN    = 0;     /* number of filled elements */

/*----------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* On the first call build the sequence once. */
    if (VALUES == NULL) {
        /* Allocate space for all 231 terms.  */
        VALUES = (seq_t *)malloc(231 * sizeof(seq_t));
        if (VALUES == NULL) /* allocation failure – return zero for all indices */
            return (seq_t)0;

        /* Prime lookup for 2‑digit numbers.  */
        static bool small_prime[100] = {0};
        for (int i = 0; i < 21; ++i)
            small_prime[PRIMES2D[i]] = true;

        /* Range of n whose squares have 16 digits.  */
        const uint64_t LOWER = 31622777ULL;      /* ceil(sqrt(1e15))   */
        const uint64_t UPPER = 99999999ULL;      /* floor(sqrt(1e16‑1)) */

        /* Brute force search.  */
        for (uint64_t x = LOWER; x <= UPPER; ++x) {
            uint64_t sq   = x * x;            /* 16‑digit square fits in 64‑bit */
            uint64_t tmp  = sq;
            bool ok = true;
            for (int block = 0; block < 8; ++block) {
                int dig = (int)(tmp % 100ULL);
                if (!small_prime[dig]) { ok = false; break; }
                tmp /= 100ULL;
            }
            if (ok) {
                VALUES[LEN++] = (seq_t)x;
                if (LEN == 231)          /* all terms found */
                    break;
            }
        }
    }

    if (n == 0 || n > LEN)          /* invalid index */
        return (seq_t)0;
    return VALUES[n - 1];            /* n‑th term (1‑based) */
}
