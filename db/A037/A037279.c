/*
 * A037279  If n is composite, replace n with the concatenation of its
 *          nontrivial divisors, otherwise a(n) = n.
 * https://oeis.org/A037279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n if n is 1 or prime, otherwise the concatenation of its
 *            non‑trivial divisors in ascending order
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 119,
 *            u128 n <= 419, big any n
 * Helpers:   digit_len, is_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* Compute a(n) for the OEIS sequence A037279. */
seq_t a(idx_t n)
{
    /* For n <= 1, the sequence is simply n. */
    if (n <= 1)
        return (seq_t)n;

    /* Composite numbers are handled specially. */
    if (is_prime(n))
        return (seq_t)n;

    /* Gather divisors: the "small" ones up to sqrt(n)
       and the complementary large ones. */
    static const size_t MAX_DIV = 256;      /* plenty for n <= 10⁶ (worst case). */
    uint64_t small[MAX_DIV];
    uint64_t large[MAX_DIV];
    size_t smallCnt = 0;
    size_t largeCnt = 0;

    for (idx_t i = 2; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            small[smallCnt++] = (uint64_t)i;

            idx_t other = n / i;
            if (i != other)
                large[largeCnt++] = (uint64_t)other;
        }
    }

    /* Concatenate the divisors in ascending order
       (small array is already ascending, large is in descending order). */
    seq_t result = 0;

    for (size_t k = 0; k < smallCnt; ++k)
    {
        seq_t d = (seq_t)small[k];
        int len = digit_len(d);
        seq_t pow10e = pow10((idx_t)len);
        result = result * pow10e + d;
    }

    /* Process large divisors in reverse to get ascending order. */
    for (size_t k = 0; k < largeCnt; ++k)
    {
        seq_t d = (seq_t)large[largeCnt - 1 - k];
        int len = digit_len(d);
        seq_t pow10e = pow10((idx_t)len);
        result = result * pow10e + d;
    }

    return result;
}
