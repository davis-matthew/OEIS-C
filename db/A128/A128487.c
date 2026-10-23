/*
 * A128487  Irregular array where n-th row is the positive integers < n which
 *          are coprime to exactly one distinct prime divisor of n.
 * https://oeis.org/A128487
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: a(n) = the n-th element of the irregular array defined in
   OEIS A128487.  For n ≥ 2, the function scans rows n = 2,3,4,…, counts
   integers m < n that are coprime to exactly one distinct prime divisor
   of n, and returns the element that corresponds to the given linear
   index.  The implementation uses simple trial‑division for prime
   factorisation and is efficient for n up to at least 10 000. */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;        /* below the offset – undefined */

    idx_t pos = n - 2;                 /* zero‑based position in the flat array */
    idx_t cumulative = 0;              /* how many terms processed so far */

    for (idx_t row = 2; ; ++row) {
        /* ---- determine distinct prime divisors of 'row' ---- */
        idx_t tmp = row;
        idx_t primes[16];              /* sufficient for row ≤ ~10⁵ */
        size_t pcount = 0;

        if (tmp % 2 == 0) {
            primes[pcount++] = 2;
            while (tmp % 2 == 0) tmp /= 2;
        }
        for (idx_t d = 3; d * d <= tmp; d += 2) {
            if (tmp % d == 0) {
                primes[pcount++] = d;
                while (tmp % d == 0) tmp /= d;
            }
        }
        if (tmp > 1) primes[pcount++] = tmp;

        /* ---- count eligible numbers < row ---- */
        idx_t len = 0;
        for (idx_t m = 1; m < row; ++m) {
            idx_t coprimeCount = 0;
            for (size_t i = 0; i < pcount; ++i) {
                if (m % primes[i] != 0) ++coprimeCount;
            }
            if (coprimeCount == 1) ++len;
        }

        /* ---- does the target lie in this row? ---- */
        if (cumulative + len > pos) {
            idx_t targetIndex = pos - cumulative;  /* 0‑based within this row */
            idx_t found = 0;
            for (idx_t m = 1; m < row; ++m) {
                idx_t coprimeCount = 0;
                for (size_t i = 0; i < pcount; ++i) {
                    if (m % primes[i] != 0) ++coprimeCount;
                }
                if (coprimeCount == 1) {
                    if (found == targetIndex) return (seq_t)m;
                    ++found;
                }
            }
            /* Should never occur */
            return (seq_t)0;
        }

        cumulative += len;
    }
}
