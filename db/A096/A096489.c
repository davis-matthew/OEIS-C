/*
 * A096489  Noncomposite numbers n such that number of decimal digits of n =
 *          number of divisors of n.
 * https://oeis.org/A096489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn, easy
 * Formula:   a(n) = 1 for n = 1; for n > 1 it equals the (n‑1)-th two‑digit
 *            prime (i.e. 11,13,17,…,97). For n beyond the 22nd term the
 *            function returns 0
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   small_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/small_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, return 0 for safety */
    if (n == 1) return (seq_t)1;   /* first term is 1 */

    idx_t target = n - 1;          /* 0‑based index among two‑digit primes */
    idx_t count = 0;

    for (uint64_t p = 11; p <= 99; ++p) {
        if (small_prime(p)) {
            ++count;
            if (count == target) return (seq_t)p;
        }
    }
    return 0;                       /* n exceeds the finite sequence length */
}
