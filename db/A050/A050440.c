/*
 * A050440  Sixth-order composites.
 * https://oeis.org/A050440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) = C(C(C(C(C(C(n))))))
 * where C(k) is the k-th composite number (C(1)=4).
 * The function below computes a(n) by iteratively applying the
 * composite-mapping six times.  It uses a simple on‑demand
 * generator based on an is_prime test, which is sufficient for
 * n up to ≈10 000 in under 2 s.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the k‑th composite number (k >= 1). */
static seq_t nth_composite(idx_t k)
{
    uint64_t count = 0;
    uint64_t num   = 4;              /* first composite */

    while (true) {
        if (!is_prime(num)) {
            ++count;
            if (count == k)
                return (seq_t)num;
        }
        ++num;
    }
}

/* External linkage required by the test harness. */
seq_t a(idx_t n)
{
    uint64_t k = (uint64_t)n;        /* local working index */

    for (int i = 0; i < 6; ++i) {
        k = (uint64_t)nth_composite(k);
    }

    return (seq_t)k;
}
