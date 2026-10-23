/*
 * A168252  Numbers n such that n is squarefree and n is non-single or
 *          nonisolated.
 * https://oeis.org/A168252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168252: squarefree integers n such that
   n is a prime with a twin prime neighbor, or
   n is a composite (squarefree) that is not sandwiched
   between two primes. (1 is always included.)
   The sequence is generated in increasing order.  */

#include <stdint.h>
#include <stdbool.h>

/* Integer primality test for uint64_t (sufficient for the
   values needed when n ≤ 10000).  */
static inline bool is_prime(uint64_t v) {
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    for (uint64_t d = 3; d <= v / d; d += 2) {
        if (v % d == 0) return false;
    }
    return true;
}

/* Test whether a 64‑bit integer is squarefree.  */
static inline bool is_squarefree(uint64_t v) {
    if (v <= 1) return true;
    for (uint64_t p = 2; p <= v / p; ++p) {
        if (v % p == 0) {
            if ((v / p) % p == 0) return false;   /* p² divides v */
            while (v % p == 0) v /= p;            /* remove p completely */
        }
    }
    return true;
}

/* Return the nth term of sequence A168252 (offset   1).  */
seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t m = 1;          /* candidate integer */

    while (1) {
        bool ok = false;

        if (m == 1) {                 /* 1 is always included */
            ok = true;
        } else {
            if (is_squarefree(m)) {
                if (is_prime(m)) {     /* prime case: needs a twin prime */
                    if (m > 2 && is_prime(m - 2))
                        ok = true;
                    else if (is_prime(m + 2))
                        ok = true;
                } else {               /* composite squarefree case */
                    bool leftP  = is_prime(m - 1);
                    bool rightP = is_prime(m + 1);
                    if (!(leftP && rightP))   /* not sandwiched by two primes */
                        ok = true;
                }
            }
        }

        if (ok) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
        ++m;
    }
}
