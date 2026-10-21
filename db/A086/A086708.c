/*
 * A086708  Primes p such that p-1 and p+1 are both divisible by cubes (other
 *          than 1).
 * https://oeis.org/A086708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p with the property that both p-1 and p+1 are divisible
 *            by a non‑trivial cube
 * Range:     u8 none, u16 n <= 228, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, grow_seq, next_prime, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq.h"
#include "../utils/next_prime.h"
#include "../utils/smallest_prime_factor.h"

/* Returns true if n has a non‑trivial cube divisor (i.e. some prime factor occurs with exponent ≥ 3). */
static bool has_cube_divisor_uint64(uint64_t n)
{
    if (n <= 1) return false;
    while (n > 1) {
        uint64_t f = smallest_prime_factor(n);
        int exp = 0;
        while (n % f == 0) {
            n /= f;
            ++exp;
            if (exp >= 3) return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset 1 */

    /* Static cache of found terms. */
    static seq_t *seq = NULL;
    static size_t sz   = 0;           /* number of terms stored */
    static size_t cap  = 0;           /* allocated capacity */
    static uint64_t last = 1;         /* last integer inspected (start from 1 so next_prime(1) = 2) */

    if (seq == NULL) {
        cap = 16;
        seq = alloc_seq_array(cap);
    }

    /* Generate terms until we have at least n of them. */
    while (sz < n) {
        uint64_t p = next_prime(last);
        last = p;

        if (has_cube_divisor_uint64(p - 1) && has_cube_divisor_uint64(p + 1)) {
            if (sz >= cap) {
                grow_seq((seq_t **)&seq, &cap, sz + 1);
            }
            seq[sz++] = (seq_t)p;
        }
    }

    return seq[n - 1];
}
