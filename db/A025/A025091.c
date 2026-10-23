/*
 * A025091  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (Lucas numbers), t = (composite numbers).
 * https://oeis.org/A025091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(1)t(n) + s(2)t(n-1) + … + s(k)t(n-k+1), where k =
 *            ⌊(n+1)/2⌋, s = Lucas numbers (L0 = 2, L1 = 1, L2 = 3, …), t =
 *            composite numbers starting at 6 (6,8,9,10,12,…)
 * Range:     u8 n <= 8, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple primality test for 64-bit values (trial division). */
static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Compute a(n) --------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* n >= 1 per OEIS offset. */
    if (n == 0) return 0;               /* safety for off‑by‑one callers */

    /* 1. generate the first n composite numbers (starting at 6). */
    size_t need = (size_t)n;
    seq_t *comps = (seq_t*)malloc(need * sizeof(seq_t));
    if (!comps) abort();                /* out of memory */

    size_t count = 0;
    uint64_t candidate = 6;             /* first composite used by the sequence */
    while (count < need) {
        if (!is_prime(candidate)) {
            comps[count++] = (seq_t)candidate;
        }
        candidate++;
    }

    /* 2. compute the required Lucas numbers and sum the convolution. */
    size_t k = (n + 1) / 2;              /* floor((n+1)/2) */
    seq_t sum  = 0;
    seq_t prev2 = 1;                     /* L1 */
    seq_t prev1 = 3;                     /* L2 */

    for (size_t i = 1; i <= k; ++i) {
        seq_t s;
        if (i == 1)
            s = prev2;
        else if (i == 2)
            s = prev1;
        else {
            s   = prev2 + prev1;        /* L_i = L_{i-1} + L_{i-2} */
            prev2 = prev1;
            prev1 = s;
        }

        /* t-index is (n - i + 1), 1‑based; convert to 0‑based. */
        idx_t t_index = n - i + 1;
        seq_t t = comps[(size_t)t_index - 1];
        sum += s * t;
    }

    free(comps);
    return sum;
}
