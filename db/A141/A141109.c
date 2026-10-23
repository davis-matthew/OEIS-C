/*
 * A141109  Even numbers 2n such that for every prime p in [n,2n-2], 2n-p is
 *          also prime.
 * https://oeis.org/A141109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   is_prime_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A141109 – even numbers 2n such that for every prime p in [n,2n-2],
    2n-p is also prime.  The sequence is finite; the largest term is 210
    (obtained for n = 105).  This implementation performs a brute‑force
    search over n from 2 to 105, using the supplied is_prime_generic
    routine.  The terms are memoised for subsequent calls. */
#include "../utils/is_prime_generic.h"

seq_t a(idx_t n) {
    /* memoised list of terms already found */
    static seq_t terms[32];
    static idx_t count = 0;        /* number of terms stored */
    static bool initialized = false;

    if (!initialized) {
        /* generate all terms up to the known maximum 210 (n = 105) */
        for (idx_t k = 2; k <= 105; ++k) {
            bool ok = true;
            idx_t upper = 2 * k - 2;
            for (idx_t p = k; ok && p <= upper; ++p) {
                if (is_prime_generic(p)) {
                    if (!is_prime_generic(2 * k - p)) {
                        ok = false;
                    }
                }
            }
            if (ok) {
                terms[count++] = (seq_t)(2 * k);
            }
        }
        initialized = true;
    }

    /* return the requested term, or 0 if n is out of range */
    if (n >= 1 && n <= count) {
        return terms[n - 1];
    }
    return (seq_t)0;
}
