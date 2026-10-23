/*
 * A036326  Composite numbers n such that juxtaposition of prime factors of n
 *          has length 2.
 * https://oeis.org/A036326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A036326: composite numbers whose
 * juxtaposition of prime factors has length 2.
 * Each such number is the product of exactly two primes from the set
 * {2,3,5,7}, ordered increasingly.  The sequence is finite (10 terms). */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The primes allowed in the factorisation. */
    static const seq_t primes[4] = { (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)7 };

    /* The sequence values will be generated once and re‑used. */
    static seq_t seq[10];
    static int initialized = 0;

    if (!initialized) {
        /* Fill in all p*q with p <= q from the prime set. */
        int k = 0;
        for (int i = 0; i < 4; ++i) {
            for (int j = i; j < 4; ++j) {
                /* seq_t guarantees the multiplication fits. */
                seq[k++] = primes[i] * primes[j];
            }
        }
        /* Sort the 10 values in ascending order. */
        for (int i = 0; i < 9; ++i) {
            for (int j = i + 1; j < 10; ++j) {
                if (seq[i] > seq[j]) {
                    seq_t tmp = seq[i];
                    seq[i] = seq[j];
                    seq[j] = tmp;
                }
            }
        }
        initialized = 1;
    }

    if (n < (idx_t)1 || n > (idx_t)10)
        return (seq_t)0;          /* undefined for indices outside the range */

    return seq[(size_t)n - 1];
}
